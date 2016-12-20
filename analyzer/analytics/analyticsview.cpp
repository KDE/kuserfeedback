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

#include "analyticsview.h"
#include "ui_analyticsview.h"

#include "aggregateddatamodel.h"
#include "chart.h"

#include <model/categoryaggregationmodel.h>
#include <model/datamodel.h>
#include <model/numericaggregationmodel.h>
#include <model/ratiosetaggregationmodel.h>
#include <model/timeaggregationmodel.h>
#include <rest/restapi.h>
#include <core/sample.h>

#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkReply>
#include <QSettings>

using namespace UserFeedback::Analyzer;

AnalyticsView::AnalyticsView(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::AnalyticsView),
    m_dataModel(new DataModel(this)),
    m_timeAggregationModel(new TimeAggregationModel(this)),
    m_aggregatedDataModel(new AggregatedDataModel(this)),
    m_chart(new Chart(this))
{
    ui->setupUi(this);

    ui->dataView->setModel(m_dataModel);
    ui->aggregatedDataView->setModel(m_aggregatedDataModel);

    m_timeAggregationModel->setSourceModel(m_dataModel);
    m_chart->setModel(m_timeAggregationModel);

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

    auto timeAggrMenu = new QMenu(tr("&Time interval"), this);
    timeAggrMenu->addAction(ui->actionAggregateDay);
    timeAggrMenu->addAction(ui->actionAggregateWeek);
    timeAggrMenu->addAction(ui->actionAggregateMonth);
    timeAggrMenu->addAction(ui->actionAggregateYear);

    connect(ui->actionReload, &QAction::triggered, m_dataModel, &DataModel::reload);
    connect(ui->actionExportData, &QAction::triggered, this, &AnalyticsView::exportData);
    connect(ui->actionImportData, &QAction::triggered, this, &AnalyticsView::importData);

    addActions({
        timeAggrMenu->menuAction(),
        ui->actionReload,
        ui->actionExportData,
        ui->actionImportData
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
}

AnalyticsView::~AnalyticsView()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("Analytics"));
    settings.setValue(QStringLiteral("TimeAggregationMode"), m_timeAggregationModel->aggregationMode());
    settings.endGroup();
}

void AnalyticsView::setRESTClient(RESTClient* client)
{
    m_client = client;
    m_dataModel->setRESTClient(client);
}

void AnalyticsView::setProduct(const Product& product)
{
    m_chart->setModel(nullptr);
    m_dataModel->setProduct(product);

    ui->chartType->clear();
    m_aggregatedDataModel->clear();
    qDeleteAll(m_aggregationModels);
    m_aggregationModels.clear();

    m_aggregatedDataModel->addSourceModel(m_timeAggregationModel);
    ui->chartType->addItem(tr("Samples"), QVariant::fromValue(m_timeAggregationModel));

    foreach (const auto &schemaEntry, product.schema()) {
        switch (schemaEntry.aggregationType()) {
            case SchemaEntry::None:
                break;
            case SchemaEntry::Category:
            {
                auto model = new CategoryAggregationModel(this);
                model->setSourceModel(m_timeAggregationModel);
                model->setAggregationValue(schemaEntry.name() + QLatin1Char('.') + schemaEntry.elements().at(0).name());
                m_aggregationModels.push_back(model);
                m_aggregatedDataModel->addSourceModel(model, schemaEntry.name());
                ui->chartType->addItem(schemaEntry.name(), QVariant::fromValue(model));
                break;
            }
            case SchemaEntry::Numeric:
            {
                auto model = new NumericAggregationModel(this);
                model->setSourceModel(m_timeAggregationModel);
                model->setAggregationValue(schemaEntry.name() + QLatin1Char('.') + schemaEntry.elements().at(0).name());
                m_aggregationModels.push_back(model);
                m_aggregatedDataModel->addSourceModel(model, schemaEntry.name());
                ui->chartType->addItem(schemaEntry.name(), QVariant::fromValue(model));
                break;
            }
            case SchemaEntry::RatioSet:
            {
                auto model = new RatioSetAggregationModel(this);
                model->setSourceModel(m_timeAggregationModel);
                model->setAggregationValue(schemaEntry.name());
                m_aggregationModels.push_back(model);
                m_aggregatedDataModel->addSourceModel(model, schemaEntry.name());
                ui->chartType->addItem(schemaEntry.name(), QVariant::fromValue(model));
                break;
            }
            case SchemaEntry::XY:
                // TODO
                break;
        }
    }
}

void AnalyticsView::exportData()
{
    const auto fileName = QFileDialog::getSaveFileName(this, tr("Export Data"));
    if (fileName.isEmpty())
        return;

    QFile f(fileName);
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::critical(this, tr("Export Failed"), tr("Could not open file: %1").arg(f.errorString()));
        return;
    }

    const auto samples = m_dataModel->index(0, 0).data(DataModel::AllSamplesRole).value<QVector<Sample>>();
    f.write(Sample::toJson(samples, m_dataModel->product()));
    emit logMessage(tr("Sample data of %1 exported to %2.").arg(m_dataModel->product().name(), f.fileName()));
}

void AnalyticsView::importData()
{
    const auto fileName = QFileDialog::getOpenFileName(this, tr("Import Data"));
    if (fileName.isEmpty())
        return;

    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        QMessageBox::critical(this, tr("Import Failed"), tr("Could not open file: %1").arg(f.errorString()));
        return;
    }
    const auto samples = Sample::fromJson(f.readAll(), m_dataModel->product());
    if (samples.isEmpty()) {
        QMessageBox::critical(this, tr("Import Failed"), tr("Selected file contains no valid data."));
        return;
    }

    auto reply = RESTApi::addSamples(m_client, m_dataModel->product(), samples);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit logMessage(tr("Samples imported."));
            m_dataModel->reload();
        }
    });
}
