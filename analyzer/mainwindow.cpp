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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connectdialog.h"
#include "datamodel.h"
#include "productmodel.h"
#include "restclient.h"
#include "serverinfo.h"
#include "surveydialog.h"
#include "surveymodel.h"
#include "timeaggregationmodel.h"

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QInputDialog>
#include <QKeySequence>
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
    m_surveyModel(new SurveyModel(this))
{
    ui->setupUi(this);
    ui->productListView->setModel(m_productModel);
    ui->dataView->setModel(m_dataModel);
    ui->surveyView->setModel(m_surveyModel);
    ui->timeAggregationMode->addItem(tr("Year"), TimeAggregationModel::AggregateYear);
    ui->timeAggregationMode->addItem(tr("Month"), TimeAggregationModel::AggregateMonth);
    ui->timeAggregationMode->addItem(tr("Week"), TimeAggregationModel::AggregateWeek);
    ui->timeAggregationMode->addItem(tr("Day"), TimeAggregationModel::AggregateDay);
    connect(ui->timeAggregationMode, &QComboBox::currentTextChanged, this, [this]() {
        m_timeAggregationModel->setAggregationMode(static_cast<TimeAggregationModel::AggregationMode>(ui->timeAggregationMode->currentData().toInt()));
    });
    ui->timeAggregationView->setModel(m_timeAggregationModel);
    setWindowIcon(QIcon::fromTheme(QStringLiteral("search")));

    connect(m_restClient, &RESTClient::errorMessage, this, &MainWindow::logError);
    m_productModel->setRESTClient(m_restClient);
    m_dataModel->setRESTClient(m_restClient);
    m_surveyModel->setRESTClient(m_restClient);
    m_timeAggregationModel->setSourceModel(m_dataModel);

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
    });

    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionQuit->setIcon(QIcon::fromTheme(QStringLiteral("application-exit")));
    connect(ui->actionQuit, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);

    connect(ui->productListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        const auto product = selectedProduct();
        if (product.isEmpty())
            return;
        m_dataModel->setProductId(product);
        m_surveyModel->setProductId(product);
    });

    QTimer::singleShot(0, ui->actionConnectToServer, &QAction::trigger);
}

MainWindow::~MainWindow() = default;

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
    auto reply = m_restClient->post(QStringLiteral("products"), product.toJson());
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
    auto reply = m_restClient->deleteResource(QStringLiteral("products/") + product.name());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            logMessage(QString::fromUtf8(reply->readAll()));
        }
        m_productModel->reload();
    });
}

void MainWindow::createSurvey()
{
    const auto product = selectedProduct();
    if (product.isEmpty())
        return;
    SurveyDialog dlg(this);
    if (!dlg.exec())
        return;
    auto reply = m_restClient->post(QStringLiteral("surveys/") + product, dlg.survey().toJson());
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
    if (product.isEmpty())
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
    ui->logWidget->append(msg);
}

void MainWindow::logError(const QString& msg)
{
    ui->logWidget->append(QStringLiteral("<font color=\"red\">"));
    ui->logWidget->append(msg);
    ui->logWidget->append(QStringLiteral("</font>"));
}

QString MainWindow::selectedProduct() const
{
    const auto selection = ui->productListView->selectionModel()->selectedRows();
    if (selection.isEmpty())
        return {};
    const auto idx = selection.first();
    return idx.data().toString();
}
