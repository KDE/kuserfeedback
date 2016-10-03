/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <config-userfeedback-version.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connectdialog.h"

#include <model/productmodel.h>

#include <rest/restapi.h>
#include <rest/restclient.h>
#include <rest/serverinfo.h>

#include <provider/widgets/feedbackconfigdialog.h>
#include <provider/core/propertyratiosource.h>
#include <provider/core/provider.h>

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QInputDialog>
#include <QKeySequence>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QTimer>
#include <QUrl>

using namespace UserFeedback::Analyzer;

MainWindow::MainWindow() :
    ui(new Ui::MainWindow),
    m_restClient(new RESTClient(this)),
    m_productModel(new ProductModel(this)),
    m_feedbackProvider(new UserFeedback::Provider(this))
{
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme(QStringLiteral("search")));

    addView(ui->surveyEditor, ui->menuSurvery);
    addView(ui->schemaEdit, ui->menuSchema);
    addView(ui->analyticsView, ui->menuAnalytics);

    ui->productListView->setModel(m_productModel);

    connect(m_restClient, &RESTClient::errorMessage, this, &MainWindow::logError);
    m_productModel->setRESTClient(m_restClient);
    ui->surveyEditor->setRESTClient(m_restClient);
    ui->analyticsView->setRESTClient(m_restClient);

    ui->actionViewAnalytics->setData(QVariant::fromValue(ui->analyticsView));
    ui->actionViewSurveys->setData(QVariant::fromValue(ui->surveyEditor));
    ui->actionViewSchema->setData(QVariant::fromValue(ui->schemaEdit));
    auto viewGroup = new QActionGroup(this);
    viewGroup->setExclusive(true);
    viewGroup->addAction(ui->actionViewAnalytics);
    viewGroup->addAction(ui->actionViewSurveys);
    viewGroup->addAction(ui->actionViewSchema);
    connect(viewGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        auto view = action->data().value<QWidget*>();
        ui->viewStack->setCurrentWidget(view);
    });
    ui->actionViewAnalytics->setChecked(true); // TODO save/restore from settings

    connect(ui->actionConnectToServer, &QAction::triggered, this, [this]() {
        QSettings settings;
        auto info = ServerInfo::load(settings.value(QStringLiteral("LastServerInfo")).toString());
        ConnectDialog dlg(this);
        dlg.setServerInfo(info);
        if (dlg.exec()) {
            info = dlg.serverInfo();
            info.save();
            settings.setValue(QStringLiteral("LastServerInfo"), info.url().toString());
            connectToServer(info);
        }
    });

    connect(ui->actionAddProduct, &QAction::triggered, this, &MainWindow::createProduct);
    connect(ui->actionDeleteProduct, &QAction::triggered, this, &MainWindow::deleteProduct);

    connect(ui->schemaEdit, &SchemaEditWidget::productChanged, this, [this](const Product &p) {
        auto reply = RESTApi::updateProduct(m_restClient, p);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            if (reply->error() != QNetworkReply::NoError)
                return;
            logMessage(QString::fromUtf8((reply->readAll())));
            m_productModel->reload();
        });
    });

    ui->actionQuit->setShortcut(QKeySequence::Quit);
    connect(ui->actionQuit, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
    connect(ui->actionContribute, &QAction::triggered, this, [this]() {
        FeedbackConfigDialog dlg(this);
        dlg.setFeedbackProvider(m_feedbackProvider);
        dlg.exec();
    });
    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About User Feedback Analyzer"), tr(
            "Version: %1\n"
            "License: LGPLv2+\n"
            "Copyright ⓒ 2016 Volker Krause <vkrause@kde.org>"
        ).arg(QStringLiteral(USERFEEDBACK_VERSION_STRING)));
    });

    connect(ui->productListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::productSelected);

    QSettings settings;
    settings.beginGroup(QStringLiteral("MainWindow"));
    restoreGeometry(settings.value(QStringLiteral("Geometry")).toByteArray());
    restoreState(settings.value(QStringLiteral("State")).toByteArray());

    connect(ui->viewStack, &QStackedWidget::currentChanged, this, &MainWindow::updateActions);
    updateActions();

    QTimer::singleShot(0, ui->actionConnectToServer, &QAction::trigger);

    m_feedbackProvider->setFeedbackServer(QUrl(QStringLiteral("https://feedback.volkerkrause.eu")));
    m_feedbackProvider->setSubmissionInterval(1);
    m_feedbackProvider->setApplicationUsageTimeUntilEncouragement(10);
    auto viewModeSource = new UserFeedback::PropertyRatioSource(ui->viewStack, "currentIndex", QStringLiteral("viewRatio"));
    viewModeSource->addValueMapping(0, QStringLiteral("analytics"));
    viewModeSource->addValueMapping(1, QStringLiteral("surveyEditor"));
    viewModeSource->addValueMapping(2, QStringLiteral("schemaEditor"));
    m_feedbackProvider->addDataSource(viewModeSource, Provider::AllStatistics);
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("MainWindow"));
    settings.setValue(QStringLiteral("State"), saveState());
    settings.setValue(QStringLiteral("Geometry"), saveGeometry());
}

template <typename T>
void MainWindow::addView(T *view, QMenu *menu)
{
    for (auto action : view->actions())
        menu->addAction(action);

    connect(view, &T::logMessage, this, &MainWindow::logMessage);
}

void MainWindow::connectToServer(const ServerInfo& info)
{
    m_restClient->connectToServer(info);
    auto reply = m_restClient->get(QStringLiteral("check_schema"));
    connect(reply, &QNetworkReply::finished, this, [this, reply, info]() {
        if (reply->error() == QNetworkReply::NoError) {
            logMessage(tr("Connected to %1.").arg(info.url().toString()));
            logMessage(QString::fromUtf8(reply->readAll()));
        } else {
            logError(reply->errorString());
            QMessageBox::critical(this, tr("Connection Failure"), tr("Failed to connect to server: %1").arg(reply->errorString()));
        }
    });
}

void MainWindow::createProduct()
{
    const auto name = QInputDialog::getText(this, tr("Add New Product"), tr("Product Identifier:"));
    if (name.isEmpty())
        return;
    Product product;
    product.setName(name);

    QVector<SchemaEntry> schema;
    SchemaEntry entry;
    entry.setName(QStringLiteral("version"));
    entry.setType(SchemaEntry::StringType);
    schema.push_back(entry);
    entry.setName(QStringLiteral("platform"));
    schema.push_back(entry);
    entry.setName(QStringLiteral("startCount"));
    entry.setType(SchemaEntry::IntegerType);
    schema.push_back(entry);
    entry.setName(QStringLiteral("usageTime"));
    schema.push_back(entry);

    product.setSchema(schema);

    auto reply = RESTApi::createProduct(m_restClient, product);
    connect(reply, &QNetworkReply::finished, this, [this, reply, name]() {
        if (reply->error() == QNetworkReply::NoError) {
            logMessage(QString::fromUtf8(reply->readAll()));
            m_productModel->reload();
        }
    });
}

void MainWindow::deleteProduct()
{
    auto sel = ui->productListView->selectionModel()->selectedRows();
    if (sel.isEmpty())
        return;
    const auto product = sel.first().data(ProductModel::ProductRole).value<Product>();
    // TODO saftey question
    auto reply = RESTApi::deleteProduct(m_restClient, product);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            logMessage(QString::fromUtf8(reply->readAll()));
        }
        m_productModel->reload();
    });
}

void MainWindow::productSelected()
{
    const auto product = selectedProduct();
    if (!product.isValid())
        return;
    ui->surveyEditor->setProduct(product);
    ui->schemaEdit->setProduct(product);
    ui->analyticsView->setProduct(product);
}

void MainWindow::logMessage(const QString& msg)
{
    ui->logWidget->appendPlainText(msg);
}

void MainWindow::logError(const QString& msg)
{
    ui->logWidget->appendHtml( QStringLiteral("<font color=\"red\">") + msg + QStringLiteral("</font>"));
}

Product MainWindow::selectedProduct() const
{
    const auto selection = ui->productListView->selectionModel()->selectedRows();
    if (selection.isEmpty())
        return {};
    const auto idx = selection.first();
    return idx.data(ProductModel::ProductRole).value<Product>();
}

void MainWindow::updateActions()
{
    // only show actions from the currently active view
    for (int i = 0; i < ui->viewStack->count(); ++i) {
        const auto w = ui->viewStack->widget(i);
        const auto isActive = ui->viewStack->currentIndex() == i;

        for (auto action : w->actions())
            action->setVisible(isActive);
    }

    // deactivate empty menus
    ui->menuAnalytics->setEnabled(!ui->menuAnalytics->isEmpty());
    ui->menuSurvery->setEnabled(!ui->menuSurvery->isEmpty());
    ui->menuSchema->setEnabled(!ui->menuSchema->isEmpty());
}
