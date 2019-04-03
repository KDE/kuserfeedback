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

using namespace KUserFeedback::Console;
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

        QObject::connect(sourceModel(), &QAbstractItemModel::modelReset, m_timelineChart.get(), [this]() {
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
