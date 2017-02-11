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

#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>

#include <QApplication>
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
    if (m_timelineChart)
        return m_timelineChart.get();

    QObject::connect(sourceModel(), &QAbstractItemModel::modelReset, sourceModel(), [this]() {
            m_timelineChart.reset();
    }, Qt::UniqueConnection);

    m_timelineChart.reset(new QChart);
    m_timelineChart->setTheme(qApp->palette().color(QPalette::Window).lightnessF() < 0.25 ? QChart::ChartThemeDark : QChart::ChartThemeLight);
    auto xAxis = new QDateTimeAxis(m_timelineChart.get());
    xAxis->setFormat(QStringLiteral("yyyy-MM-dd")); // TODO, follow aggregation mode
    auto yAxis = new QValueAxis(m_timelineChart.get());
    m_timelineChart->addAxis(xAxis, Qt::AlignBottom);
    m_timelineChart->addAxis(yAxis, Qt::AlignLeft);

    auto series = new QLineSeries(m_timelineChart.get());
    series->setName(displayName());
    auto mapper = new QVXYModelMapper(series);
    mapper->setModel(timeAggregationModel());
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setFirstRow(0);
    mapper->setSeries(series);
    m_timelineChart->addSeries(series);

    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    xAxis->setTickCount(std::min(timeAggregationModel()->rowCount(), 12));
    yAxis->setMin(0);
    yAxis->setMinorTickCount(4);

    return m_timelineChart.get();
}
