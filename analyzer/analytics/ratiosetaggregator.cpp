/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include "ratiosetaggregator.h"
#include "chartutil.h"

#include <model/ratiosetaggregationmodel.h>

#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>

using namespace UserFeedback::Analyzer;
using namespace QtCharts;

RatioSetAggregator::RatioSetAggregator() = default;
RatioSetAggregator::~RatioSetAggregator() = default;

Aggregator::ChartModes RatioSetAggregator::chartModes() const
{
    Aggregator::ChartModes modes = None;
    if (aggregation().elements().size() == 1)
        modes |= Timeline;
    return modes;
}

QString RatioSetAggregator::displayName() const
{
    const auto e = aggregation().elements().at(0);
    return e.schemaEntry().name();
}

QAbstractItemModel* RatioSetAggregator::timeAggregationModel()
{
    if (!m_model && !aggregation().elements().isEmpty()) {
        m_model.reset(new RatioSetAggregationModel);
        m_model->setSourceModel(sourceModel());
        const auto e = aggregation().elements().at(0);
        m_model->setAggregationValue(e.schemaEntry().name());
        QObject::connect(m_model.get(), &QAbstractItemModel::modelReset, [this]() {
                m_timelineChart.reset();
        });
    }
    return m_model.get();
}

QtCharts::QChart* RatioSetAggregator::timelineChart()
{
    if (m_timelineChart)
        return m_timelineChart.get();

    m_timelineChart.reset(new QChart);
    ChartUtil::applyTheme(m_timelineChart.get());
    auto xAxis = new QDateTimeAxis(m_timelineChart.get());
    xAxis->setFormat(QStringLiteral("yyyy-MM-dd")); // TODO, follow aggregation mode
    auto yAxis = new QValueAxis(m_timelineChart.get());
    m_timelineChart->addAxis(xAxis, Qt::AlignBottom);
    m_timelineChart->addAxis(yAxis, Qt::AlignLeft);

    QLineSeries *prevSeries = nullptr;
    for (int i = 1; i < timeAggregationModel()->columnCount(); ++i) {
        auto series = new QLineSeries;

        auto mapper = new QVXYModelMapper(series);
        mapper->setModel(timeAggregationModel());
        mapper->setXColumn(0);
        mapper->setYColumn(i);
        mapper->setFirstRow(0);
        mapper->setSeries(series);

        auto areaSeries = new QAreaSeries(series, prevSeries);
        series->setParent(areaSeries); // otherwise series isn't deleted by removeAllSeries!
        areaSeries->setName(timeAggregationModel()->headerData(i, Qt::Horizontal).toString().toHtmlEscaped());
        m_timelineChart->addSeries(areaSeries);

        areaSeries->attachAxis(xAxis);
        areaSeries->attachAxis(yAxis);

        prevSeries = series;
    }

    xAxis->setTickCount(std::min(timeAggregationModel()->rowCount(), 12));
    yAxis->setMin(0);
    yAxis->setMax(1); // TODO can we turn this into *100% for display?
    yAxis->setTickCount(11);

    return m_timelineChart.get();
}
