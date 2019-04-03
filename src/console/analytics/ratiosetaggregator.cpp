/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ratiosetaggregator.h"
#include "chartutil.h"

#include <model/extrarowsproxymodel.h>
#include <model/ratiosetaggregationmodel.h>
#include <model/rolemappingproxymodel.h>
#include <model/timeaggregationmodel.h>

#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QHPieModelMapper>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>

using namespace KUserFeedback::Console;
using namespace QtCharts;

RatioSetAggregator::RatioSetAggregator() = default;
RatioSetAggregator::~RatioSetAggregator() = default;

Aggregator::ChartModes RatioSetAggregator::chartModes() const
{
    Aggregator::ChartModes modes = None;
    if (aggregation().elements().size() == 1)
        modes |= Timeline | Singular;
    return modes;
}

QAbstractItemModel* RatioSetAggregator::timeAggregationModel()
{
    if (!m_model && !aggregation().elements().isEmpty()) {
        m_model.reset(new RatioSetAggregationModel);
        m_model->setSourceModel(sourceModel());
        const auto e = aggregation().elements().at(0);
        m_model->setAggregationValue(e.schemaEntry().name());
        QObject::connect(m_model.get(), &QAbstractItemModel::modelReset, [this]() {
            updateTimelineChart();
            updateSingularChart();
        });
    }
    return m_model.get();
}

QtCharts::QChart* RatioSetAggregator::timelineChart()
{
    if (!m_timelineChart) {
        m_timelineChart.reset(new QChart);
        ChartUtil::applyTheme(m_timelineChart.get());
        auto xAxis = new QDateTimeAxis(m_timelineChart.get());
        xAxis->setFormat(QStringLiteral("yyyy-MM-dd")); // TODO, follow aggregation mode
        auto yAxis = new QValueAxis(m_timelineChart.get());
        yAxis->setTickCount(11);
        m_timelineChart->addAxis(xAxis, Qt::AlignBottom);
        m_timelineChart->addAxis(yAxis, Qt::AlignLeft);
        updateTimelineChart();
    }

    return m_timelineChart.get();
}

void RatioSetAggregator::updateTimelineChart()
{
    if (!m_timelineChart)
        return;
    m_timelineChart->removeAllSeries();

    auto model = new RoleMappingProxyModel(m_timelineChart.get());
    model->setSourceModel(timeAggregationModel());
    model->addRoleMapping(Qt::DisplayRole, TimeAggregationModel::AccumulatedDisplayRole);

    QLineSeries *prevSeries = nullptr;
    for (int i = 1; i < timeAggregationModel()->columnCount(); ++i) {
        auto series = new QLineSeries;

        auto mapper = new QVXYModelMapper(series);
        mapper->setModel(model);
        mapper->setXColumn(0);
        mapper->setYColumn(i);
        mapper->setFirstRow(0);
        mapper->setSeries(series);

        auto areaSeries = new QAreaSeries(series, prevSeries);
        series->setParent(areaSeries); // otherwise series isn't deleted by removeAllSeries!
        areaSeries->setName(model->headerData(i, Qt::Horizontal).toString().toHtmlEscaped());
        m_timelineChart->addSeries(areaSeries);

        areaSeries->attachAxis(m_timelineChart->axisX());
        areaSeries->attachAxis(m_timelineChart->axisY());

        prevSeries = series;
    }

    qobject_cast<QDateTimeAxis*>(m_timelineChart->axisX())->setTickCount(std::min(model->rowCount(), 12));
    m_timelineChart->axisY()->setRange(0, 1); // TODO can we turn this into *100% for display?
}

QtCharts::QChart* RatioSetAggregator::singlularChart()
{
    if (!m_singularChart) {
        m_singularChart.reset(new QChart);
        ChartUtil::applyTheme(m_singularChart.get());
        updateSingularChart();
    }

    return m_singularChart.get();
}

void RatioSetAggregator::updateSingularChart()
{
    if (!m_singularChart)
        return;
    m_singularChart->removeAllSeries();

    if (sourceModel()->rowCount() <= 0)
        return;

    auto series = new QPieSeries(m_singularChart.get());
    auto mapper = new QHPieModelMapper(m_singularChart.get());
    auto modelWithLabels = new ExtraRowsProxyModel(mapper);
    modelWithLabels->setSourceModel(singularAggregationModel());
    mapper->setModel(modelWithLabels);
    mapper->setFirstColumn(1);
    mapper->setValuesRow(0);
    mapper->setLabelsRow(1);
    mapper->setSeries(series);

    decoratePieSeries(series);
    QObject::connect(series, &QPieSeries::added, [this, series]() {
        decoratePieSeries(series);
    });

    m_singularChart->addSeries(series);
}

void RatioSetAggregator::decoratePieSeries(QtCharts::QPieSeries *series) const
{
    for (auto slice : series->slices()) {
        if (slice->value() > 0.01)
            slice->setLabelVisible(true);
    }
}
