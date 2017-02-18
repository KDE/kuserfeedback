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

#include "aggregator.h"
#include "categoryaggregator.h"
#include "chartutil.h"
#include "numericaggregator.h"
#include "ratiosetaggregator.h"
#include "totalaggregator.h"

#include <model/aggregateddatamodel.h>
#include <model/datamodel.h>
#include <model/timeaggregationmodel.h>
#include <rest/restapi.h>
#include <core/aggregation.h>
#include <core/sample.h>

#include <QtCharts/QChart>

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
    m_nullSingularChart(new QtCharts::QChart),
    m_nullTimelineChart(new QtCharts::QChart)
{
    ui->setupUi(this);

    ChartUtil::applyTheme(m_nullSingularChart.get());
    ChartUtil::applyTheme(m_nullTimelineChart.get());
    ui->singularChartView->setChart(m_nullSingularChart.get());
    ui->timelineChartView->setChart(m_nullTimelineChart.get());

    ui->dataView->setModel(m_dataModel);
    ui->aggregatedDataView->setModel(m_aggregatedDataModel);

    m_timeAggregationModel->setSourceModel(m_dataModel);
    connect(m_timeAggregationModel, &QAbstractItemModel::modelReset, this, &AnalyticsView::updateTimeSliderRange);

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

    auto chartModeGroup = new QActionGroup(this);
    chartModeGroup->addAction(ui->actionSingularChart);
    chartModeGroup->addAction(ui->actionTimelineChart);
    connect(chartModeGroup, &QActionGroup::triggered, this, &AnalyticsView::updateChart);

    auto chartMode = new QMenu(tr("&Chart mode"), this);
    chartMode->addAction(ui->actionSingularChart);
    chartMode->addAction(ui->actionTimelineChart);

    connect(ui->actionReload, &QAction::triggered, m_dataModel, &DataModel::reload);
    connect(ui->actionExportData, &QAction::triggered, this, &AnalyticsView::exportData);
    connect(ui->actionImportData, &QAction::triggered, this, &AnalyticsView::importData);

    addActions({
        timeAggrMenu->menuAction(),
        chartMode->menuAction(),
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

    connect(ui->chartType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AnalyticsView::chartSelected);
    connect(ui->timeSlider, &QSlider::valueChanged, this, [this](int value) {
        auto aggr = ui->chartType->currentData().value<Aggregator*>();
        if (aggr)
            aggr->setSingularTime(value);
    });
}

AnalyticsView::~AnalyticsView()
{
    if (ui->singularChartView->chart())
        disconnect(ui->singularChartView->chart(), &QObject::destroyed, this, &AnalyticsView::updateChart);
    if (ui->timelineChartView->chart())
        disconnect(ui->timelineChartView->chart(), &QObject::destroyed, this, &AnalyticsView::updateChart);

    QSettings settings;
    settings.beginGroup(QStringLiteral("Analytics"));
    settings.setValue(QStringLiteral("TimeAggregationMode"), m_timeAggregationModel->aggregationMode());
    settings.endGroup();

    // the chart views can't handle null or deleted charts, so set them to something safe
    ui->singularChartView->setChart(m_nullSingularChart.get());
    ui->timelineChartView->setChart(m_nullTimelineChart.get());
    qDeleteAll(m_aggregators);
}

void AnalyticsView::setRESTClient(RESTClient* client)
{
    m_client = client;
    m_dataModel->setRESTClient(client);
}

void AnalyticsView::setProduct(const Product& product)
{
    // the chart views can't handle null or deleted charts, so set them to something safe
    ui->singularChartView->setChart(m_nullSingularChart.get());
    ui->timelineChartView->setChart(m_nullTimelineChart.get());

    m_dataModel->setProduct(product);

    ui->chartType->clear();
    m_aggregatedDataModel->clear();

    qDeleteAll(m_aggregators);
    m_aggregators.clear();

    m_aggregatedDataModel->addSourceModel(m_timeAggregationModel);
    auto totalsAggr = new TotalAggregator;
    totalsAggr->setSourceModel(m_timeAggregationModel);
    ui->chartType->addItem(tr("Samples"), QVariant::fromValue<Aggregator*>(totalsAggr));

    foreach (const auto &aggr, product.aggregations()) {
        auto aggregator = createAggregator(aggr);
        if (!aggregator)
            continue;
        m_aggregators.push_back(aggregator);
        if (auto model = aggregator->timeAggregationModel()) {
            m_aggregatedDataModel->addSourceModel(model, aggregator->displayName());
        }
        if (aggregator->chartModes() != Aggregator::None)
            ui->chartType->addItem(aggregator->displayName(), QVariant::fromValue(aggregator));
    }
}

void AnalyticsView::chartSelected()
{
    auto aggr = ui->chartType->currentData().value<Aggregator*>();
    if (!aggr)
        return;

    const auto chartMode = aggr->chartModes();
    ui->actionSingularChart->setEnabled(chartMode & Aggregator::Singular);
    ui->actionTimelineChart->setEnabled(chartMode & Aggregator::Timeline);
    if (chartMode != (Aggregator::Timeline | Aggregator::Singular)) {
        ui->actionSingularChart->setChecked(chartMode & Aggregator::Singular);
        ui->actionTimelineChart->setChecked(chartMode & Aggregator::Timeline);
    }

    updateChart();
}

void AnalyticsView::updateChart()
{
    auto aggr = ui->chartType->currentData().value<Aggregator*>();
    if (!aggr)
        return;

    if (ui->singularChartView->chart())
        disconnect(ui->singularChartView->chart(), &QObject::destroyed, this, &AnalyticsView::updateChart);
    if (ui->timelineChartView->chart())
        disconnect(ui->timelineChartView->chart(), &QObject::destroyed, this, &AnalyticsView::updateChart);

    if (ui->actionTimelineChart->isChecked()) {
        ui->timelineChartView->setChart(aggr->timelineChart());
        connect(ui->timelineChartView->chart(), &QObject::destroyed, this, &AnalyticsView::updateChart);
        ui->chartStack->setCurrentWidget(ui->timelinePage);
    } else if (ui->actionSingularChart->isChecked()) {
        ui->singularChartView->setChart(aggr->singlularChart());
        connect(ui->singularChartView->chart(), &QObject::destroyed, this, &AnalyticsView::updateChart);
        ui->chartStack->setCurrentWidget(ui->singularPage);
    }
}

void AnalyticsView::updateTimeSliderRange()
{
    qDebug() << m_timeAggregationModel->rowCount() << ui->timeSlider->maximum() << ui->timeSlider->minimum();
    if (m_timeAggregationModel->rowCount() > 0)
        ui->timeSlider->setRange(0, m_timeAggregationModel->rowCount() - 1);
}

Aggregator* AnalyticsView::createAggregator(const Aggregation& aggr) const
{
    Aggregator *aggregator = nullptr;

    switch (aggr.type()) {
        case Aggregation::None:
            break;
        case Aggregation::Category:
            aggregator = new CategoryAggregator;
            break;
        case Aggregation::Numeric:
            aggregator = new NumericAggregator;
            break;
        case Aggregation::RatioSet:
            aggregator = new RatioSetAggregator;
            break;
        case Aggregation::XY:
            break;
    }

    if (!aggregator)
        return nullptr;

    aggregator->setAggregation(aggr);
    aggregator->setSourceModel(m_timeAggregationModel);
    return aggregator;
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
