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

#include "totalaggregator.h"
#include "chartutil.h"

#include <model/timeaggregationmodel.h>

#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>

#include <QAbstractItemModel>

#include <numeric>

using namespace UserFeedback::Analyzer;
using namespace QtCharts;

TotalAggregator::TotalAggregator() = default;
TotalAggregator::~TotalAggregator() = default;

Aggregator::ChartModes TotalAggregator::chartModes() const
{
    return Timeline;
}

QString TotalAggregator::displayName() const
{
    return tr("Samples");
}

QAbstractItemModel* TotalAggregator::timeAggregationModel()
{
    return sourceModel();
}

QtCharts::QChart* TotalAggregator::timelineChart()
{
    if (!m_timelineChart) {
        m_timelineChart.reset(new QChart);
        ChartUtil::applyTheme(m_timelineChart.get());
        auto xAxis = new QDateTimeAxis(m_timelineChart.get());
        xAxis->setFormat(QStringLiteral("yyyy-MM-dd")); // TODO, follow aggregation mode
        auto yAxis = new QValueAxis(m_timelineChart.get());
        yAxis->setMinorTickCount(4);
        m_timelineChart->addAxis(xAxis, Qt::AlignBottom);
        m_timelineChart->addAxis(yAxis, Qt::AlignLeft);
        updateTimelineChart();

        QObject::connect(sourceModel(), &QAbstractItemModel::modelReset, [this]() {
            updateTimelineChart();
        });
    }

    return m_timelineChart.get();
}

void TotalAggregator::updateTimelineChart()
{
    if (!m_timelineChart)
        return;
    m_timelineChart->removeAllSeries();
    if (timeAggregationModel()->rowCount() <= 0)
        return;

    auto series = new QLineSeries(m_timelineChart.get());
    series->setName(displayName());
    auto mapper = new QVXYModelMapper(series);
    mapper->setModel(timeAggregationModel());
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setFirstRow(0);
    mapper->setSeries(series);
    m_timelineChart->addSeries(series);

    series->attachAxis(m_timelineChart->axisX());
    series->attachAxis(m_timelineChart->axisY());

    const auto beginDt = timeAggregationModel()->index(0, 0).data(TimeAggregationModel::DateTimeRole).toDateTime();
    const auto endDt = timeAggregationModel()->index(timeAggregationModel()->rowCount() - 1, 0).data(TimeAggregationModel::DateTimeRole).toDateTime();
    m_timelineChart->axisX()->setRange(beginDt, endDt);
    qobject_cast<QDateTimeAxis*>(m_timelineChart->axisX())->setTickCount(std::min(timeAggregationModel()->rowCount(), 12));
    const auto max = timeAggregationModel()->index(0, 0).data(TimeAggregationModel::MaximumValueRole).toInt();
    m_timelineChart->axisY()->setRange(0, max);
    qobject_cast<QValueAxis*>(m_timelineChart->axisY())->applyNiceNumbers();
}
