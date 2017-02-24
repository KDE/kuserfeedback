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
#include <provider/core/applicationversionsource.h>
#include <provider/core/platforminfosource.h>
#include <provider/core/propertyratiosource.h>
#include <provider/core/provider.h>
#include <provider/core/qtversionsource.h>
#include <provider/core/startcountsource.h>
#include <provider/core/usagetimesource.h>

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

using namespace UserFeedback::Console;

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
    ui->actionViewAnalytics->setChecked(true);

    connect(ui->actionConnectToServer, &QAction::triggered, this, [this]() {
        QSettings settings;
        auto info = ServerInfo::load(settings.value(QStringLiteral("LastServerInfo")).toString());
        ConnectDialog dlg(this);
        auto recents = settings.value(QStringLiteral("RecentServerInfos")).toStringList();
        dlg.addRecentServerInfos(recents);
        dlg.setServerInfo(info);
        if (dlg.exec()) {
            info = dlg.serverInfo();
            info.save();
            settings.setValue(QStringLiteral("LastServerInfo"), info.url().toString());
            if (!recents.contains(info.url().toString())) {
                recents.push_back(info.url().toString());
                settings.setValue(QStringLiteral("RecentServerInfos"), recents);
            }
            connectToServer(info);
        }
    });

    connect(ui->actionAddProduct, &QAction::triggered, this, &MainWindow::createProduct);
    connect(ui->actionDeleteProduct, &QAction::triggered, this, &MainWindow::deleteProduct);

    connect(ui->schemaEdit, &SchemaEditor::productChanged, m_productModel, &ProductModel::reload);

    ui->actionQuit->setShortcut(QKeySequence::Quit);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
    ui->menuWindow->addAction(ui->productsDock->toggleViewAction());
    ui->menuWindow->addAction(ui->logDock->toggleViewAction());
    connect(ui->actionContribute, &QAction::triggered, this, [this]() {
        FeedbackConfigDialog dlg(this);
        dlg.setFeedbackProvider(m_feedbackProvider);
        dlg.exec();
    });
    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About User Feedback Console"), tr(
            "Version: %1\n"
            "License: LGPLv2+\n"
            "Copyright ⓒ 2016 Volker Krause <vkrause@kde.org>"
        ).arg(QStringLiteral(USERFEEDBACK_VERSION_STRING)));
    });

    connect(ui->productListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::productSelected);
    connect(m_productModel, &QAbstractItemModel::dataChanged, this, &MainWindow::productSelected);

    connect(ui->viewStack, &QStackedWidget::currentChanged, this, &MainWindow::updateActions);
    connect(ui->viewStack, &QStackedWidget::currentChanged, this, [viewGroup](int index) {
        viewGroup->actions().at(index)->setChecked(true);
    });
    updateActions();

    QSettings settings;
    settings.beginGroup(QStringLiteral("MainWindow"));
    restoreGeometry(settings.value(QStringLiteral("Geometry")).toByteArray());
    restoreState(settings.value(QStringLiteral("State")).toByteArray());
    ui->viewStack->setCurrentIndex(settings.value(QStringLiteral("CurrentView"), 0).toInt());

    QTimer::singleShot(0, ui->actionConnectToServer, &QAction::trigger);

    m_feedbackProvider->setFeedbackServer(QUrl(QStringLiteral("https://feedback.volkerkrause.eu")));
    m_feedbackProvider->setSubmissionInterval(1);
    m_feedbackProvider->setApplicationUsageTimeUntilEncouragement(10);
    auto viewModeSource = new UserFeedback::PropertyRatioSource(ui->viewStack, "currentIndex", QStringLiteral("viewRatio"));
    viewModeSource->setDescription(tr("Usage ratio of the analytics view, survey editor and schema editor."));
    viewModeSource->addValueMapping(0, QStringLiteral("analytics"));
    viewModeSource->addValueMapping(1, QStringLiteral("surveyEditor"));
    viewModeSource->addValueMapping(2, QStringLiteral("schemaEditor"));
    m_feedbackProvider->addDataSource(viewModeSource, Provider::DetailedUsageStatistics);
    m_feedbackProvider->addDataSource(new ApplicationVersionSource, Provider::BasicSystemInformation);
    m_feedbackProvider->addDataSource(new PlatformInfoSource, Provider::BasicSystemInformation);
    m_feedbackProvider->addDataSource(new QtVersionSource, Provider::BasicSystemInformation);
    m_feedbackProvider->addDataSource(new StartCountSource, Provider::BasicUsageStatistics);
    m_feedbackProvider->addDataSource(new UsageTimeSource, Provider::BasicUsageStatistics);
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("MainWindow"));
    settings.setValue(QStringLiteral("State"), saveState());
    settings.setValue(QStringLiteral("Geometry"), saveGeometry());
    settings.setValue(QStringLiteral("CurrentView"), ui->viewStack->currentIndex());
}

template <typename T>
void MainWindow::addView(T *view, QMenu *menu)
{
    for (auto action : view->actions())
        menu->addAction(action);

    view->setRESTClient(m_restClient);
    connect(view, &T::logMessage, this, &MainWindow::logMessage);
}

void MainWindow::connectToServer(const ServerInfo& info)
{
    m_restClient->connectToServer(info);
    auto reply = RESTApi::checkSchema(m_restClient);
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
    const auto mb = QMessageBox::critical(this, tr("Delete Product"),
                        tr("Do you really want to delete product %1 with all its data?").arg(product.name()),
                        QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
    if (mb != QMessageBox::Discard)
        return;

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

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (ui->schemaEdit->isDirty()) {
        const auto r = QMessageBox::critical(this, tr("Unsaved Changes"),
            tr("There are unsaved changes in the schema editor. Do you want to discard them and close the application?"),
            QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
        if (r != QMessageBox::Discard) {
            event->ignore();
            return;
        }
    }
    return QMainWindow::closeEvent(event);
}
