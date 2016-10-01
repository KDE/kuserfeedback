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

#include "aggregateddatamodel.h"
#include "categoryaggregationmodel.h"
#include "chart.h"
#include "connectdialog.h"
#include "datamodel.h"
#include "numericaggregationmodel.h"
#include "ratiosetaggregationmodel.h"
#include "surveydialog.h"
#include "surveymodel.h"
#include "timeaggregationmodel.h"

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
    m_dataModel(new DataModel(this)),
    m_timeAggregationModel(new TimeAggregationModel(this)),
    m_aggregatedDataModel(new AggregatedDataModel(this)),
    m_surveyModel(new SurveyModel(this)),
    m_chart(new Chart(this)),
    m_feedbackProvider(new UserFeedback::Provider(this))
{
    ui->setupUi(this);
    ui->productListView->setModel(m_productModel);
    ui->dataView->setModel(m_dataModel);
    ui->surveyView->setModel(m_surveyModel);
    ui->aggregatedDataView->setModel(m_aggregatedDataModel);
    setWindowIcon(QIcon::fromTheme(QStringLiteral("search")));

    connect(m_restClient, &RESTClient::errorMessage, this, &MainWindow::logError);
    m_productModel->setRESTClient(m_restClient);
    m_dataModel->setRESTClient(m_restClient);
    m_surveyModel->setRESTClient(m_restClient);
    m_timeAggregationModel->setSourceModel(m_dataModel);

    m_chart->setModel(m_timeAggregationModel);

    ui->actionConnectToServer->setIcon(QIcon::fromTheme(QStringLiteral("network-connect")));
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

    ui->actionAddProduct->setIcon(QIcon::fromTheme(QStringLiteral("folder-add")));
    connect(ui->actionAddProduct, &QAction::triggered, this, &MainWindow::createProduct);
    ui->actionDeleteProduct->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));
    connect(ui->actionDeleteProduct, &QAction::triggered, this, &MainWindow::deleteProduct);

    ui->actionAddSurvey->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    connect(ui->actionAddSurvey, &QAction::triggered, this, &MainWindow::createSurvey);
    ui->actionDeleteSurvey->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    connect(ui->actionDeleteSurvey, &QAction::triggered, this, &MainWindow::deleteSurvey);

    ui->actionAggregateYear->setData(TimeAggregationModel::AggregateYear);
    ui->actionAggregateMonth->setData(TimeAggregationModel::AggregateMonth);
    ui->actionAggregateWeek->setData(TimeAggregationModel::AggregateWeek);
    ui->actionAggregateDay->setData(TimeAggregationModel::AggregateDay);
    auto aggrGroup = new QActionGroup(this);
    aggrGroup->addAction(ui->actionAggregateYear);
    aggrGroup->addAction(ui->actionAggregateMonth);
    aggrGroup->addAction(ui->actionAggregateWeek);
    aggrGroup->addAction(ui->actionAggregateDay);
    aggrGroup->setExclusive(true);
    connect(aggrGroup, &QActionGroup::triggered, this, [this, aggrGroup]() {
        m_timeAggregationModel->setAggregationMode(static_cast<TimeAggregationModel::AggregationMode>(aggrGroup->checkedAction()->data().toInt()));
    });

    QSettings settings;
    settings.beginGroup(QStringLiteral("Analytics"));
    const auto aggrSetting = settings.value(QStringLiteral("TimeAggregationMode"), TimeAggregationModel::AggregateMonth).toInt();
    foreach (auto act, aggrGroup->actions())
        act->setChecked(act->data().toInt() == aggrSetting);
    m_timeAggregationModel->setAggregationMode(static_cast<TimeAggregationModel::AggregationMode>(aggrSetting));
    settings.endGroup();

    ui->chartView->setChart(m_chart->chart());
    connect(ui->chartType, &QComboBox::currentTextChanged, this, [this]() {
        const auto model = ui->chartType->currentData().value<QAbstractItemModel*>();
        m_chart->setModel(model);
    });

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
    ui->actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit")));
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

    settings.beginGroup(QStringLiteral("MainWindow"));
    restoreGeometry(settings.value(QStringLiteral("Geometry")).toByteArray());
    restoreState(settings.value(QStringLiteral("State")).toByteArray());

    QTimer::singleShot(0, ui->actionConnectToServer, &QAction::trigger);

    m_feedbackProvider->setFeedbackServer(QUrl(QStringLiteral("https://feedback.volkerkrause.eu")));
    m_feedbackProvider->setSubmissionInterval(1);
    m_feedbackProvider->setApplicationUsageTimeUntilEncouragement(10);
    auto viewModeSource = new UserFeedback::PropertyRatioSource(ui->tabWidget, "currentIndex", QStringLiteral("viewRatio"));
    viewModeSource->addValueMapping(0, QStringLiteral("chart"));
    viewModeSource->addValueMapping(1, QStringLiteral("aggregated_data"));
    viewModeSource->addValueMapping(2, QStringLiteral("raw_data"));
    viewModeSource->addValueMapping(3, QStringLiteral("surveys"));
    viewModeSource->addValueMapping(4, QStringLiteral("schema"));
    m_feedbackProvider->addDataSource(viewModeSource, Provider::AllStatistics);
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("Analytics"));
    settings.setValue(QStringLiteral("TimeAggregationMode"), m_timeAggregationModel->aggregationMode());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("MainWindow"));
    settings.setValue(QStringLiteral("State"), saveState());
    settings.setValue(QStringLiteral("Geometry"), saveGeometry());
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

    QVector<ProductSchemaEntry> schema;
    ProductSchemaEntry entry;
    entry.setName(QStringLiteral("version"));
    entry.setType(ProductSchemaEntry::StringType);
    schema.push_back(entry);
    entry.setName(QStringLiteral("platform"));
    schema.push_back(entry);
    entry.setName(QStringLiteral("startCount"));
    entry.setType(ProductSchemaEntry::IntegerType);
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
    m_dataModel->setProduct(product);
    m_surveyModel->setProductId(product.name());
    ui->schemaEdit->setProduct(product);

    m_chart->setModel(nullptr);
    ui->chartType->clear();
    m_aggregatedDataModel->clear();
    qDeleteAll(m_aggregationModels);
    m_aggregationModels.clear();

    m_aggregatedDataModel->addSourceModel(m_timeAggregationModel);
    ui->chartType->addItem(tr("Samples"), QVariant::fromValue(m_timeAggregationModel));

    foreach (const auto &schemaEntry, product.schema()) {
        switch (schemaEntry.type()) {
            case ProductSchemaEntry::InvalidType:
            case ProductSchemaEntry::StringListType:
                break;
            case ProductSchemaEntry::StringType:
            {
                auto model = new CategoryAggregationModel(this);
                model->setSourceModel(m_timeAggregationModel);
                model->setAggregationValue(schemaEntry.name());
                m_aggregationModels.push_back(model);
                m_aggregatedDataModel->addSourceModel(model, schemaEntry.name());
                ui->chartType->addItem(schemaEntry.name(), QVariant::fromValue(model));
                break;
            }
            case ProductSchemaEntry::IntegerType:
            {
                auto model = new NumericAggregationModel(this);
                model->setSourceModel(m_timeAggregationModel);
                model->setAggregationValue(schemaEntry.name());
                m_aggregationModels.push_back(model);
                m_aggregatedDataModel->addSourceModel(model, schemaEntry.name());
                ui->chartType->addItem(schemaEntry.name(), QVariant::fromValue(model));
                break;
            }
            case ProductSchemaEntry::RatioSetType:
            {
                auto model = new RatioSetAggregationModel(this);
                model->setSourceModel(m_timeAggregationModel);
                model->setAggregationValue(schemaEntry.name());
                m_aggregationModels.push_back(model);
                m_aggregatedDataModel->addSourceModel(model, schemaEntry.name());
                ui->chartType->addItem(schemaEntry.name(), QVariant::fromValue(model));
                break;
            }
        }
    }
}

void MainWindow::createSurvey()
{
    const auto product = selectedProduct();
    if (!product.isValid())
        return;
    SurveyDialog dlg(this);
    if (!dlg.exec())
        return;
    auto reply = m_restClient->post(QStringLiteral("surveys/") + product.name(), dlg.survey().toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            logMessage(QString::fromUtf8(reply->readAll()));
        }
        m_surveyModel->reload();
    });
}

void MainWindow::deleteSurvey()
{
    const auto product = selectedProduct();
    if (!product.isValid())
        return;
    // TODO safety check
    const auto selection = ui->surveyView->selectionModel()->selectedRows();
    if (selection.isEmpty())
        return;
    const auto survey = selection.first().data(SurveyModel::SurveyRole).value<Survey>();
    if (survey.id() < 0)
        return;
    auto reply = m_restClient->deleteResource(QStringLiteral("surveys/") + QString::number(survey.id()));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            return;
        logMessage(QString::fromUtf8(reply->readAll()));
        m_surveyModel->reload();
    });
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
