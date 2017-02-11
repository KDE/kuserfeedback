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

#include "numericaggregator.h"

#include <model/numericaggregationmodel.h>
#include <model/timeaggregationmodel.h>

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBoxPlotSeries>
#include <QtCharts/QChart>
#include <QtCharts/QHBoxPlotModelMapper>
#include <QtCharts/QValueAxis>

#include <QApplication>
#include <QDateTime>

using namespace UserFeedback::Analyzer;
using namespace QtCharts;

NumericAggregator::NumericAggregator() = default;
NumericAggregator::~NumericAggregator() = default;

Aggregator::ChartModes NumericAggregator::chartModes() const
{
    Aggregator::ChartModes modes = None;
    if (aggregation().elements().size() == 1)
        modes |= Timeline;
    return modes;
}

QString NumericAggregator::displayName() const
{
    const auto e = aggregation().elements().at(0);
    return e.schemaEntry().name();
}

QAbstractItemModel* NumericAggregator::timeAggregationModel()
{
    if (!m_model && !aggregation().elements().isEmpty()) {
        m_model.reset(new NumericAggregationModel);
        m_model->setSourceModel(sourceModel());
        const auto e = aggregation().elements().at(0);
        m_model->setAggregationValue(e.schemaEntry().name() + QLatin1Char('.') + e.schemaEntryElement().name());
        QObject::connect(m_model.get(), &QAbstractItemModel::modelReset, [this]() {
            m_timelineChart.reset();
        });
    }
    return m_model.get();
}

QtCharts::QChart* NumericAggregator::timelineChart()
{
    if (m_timelineChart)
        return m_timelineChart.get();

    m_timelineChart.reset(new QChart);
    m_timelineChart->setTheme(qApp->palette().color(QPalette::Window).lightnessF() < 0.25 ? QChart::ChartThemeDark : QChart::ChartThemeLight);
    auto xAxis = new QBarCategoryAxis(m_timelineChart.get());
    auto yAxis = new QValueAxis(m_timelineChart.get());
    m_timelineChart->addAxis(xAxis, Qt::AlignBottom);
    m_timelineChart->addAxis(yAxis, Qt::AlignLeft);

    auto series = new QBoxPlotSeries(m_timelineChart.get());
    series->setName(displayName());
    auto mapper = new QHBoxPlotModelMapper(series);
    mapper->setModel(timeAggregationModel());
    mapper->setFirstColumn(1);
    mapper->setFirstBoxSetRow(0);
    mapper->setLastBoxSetRow(timeAggregationModel()->rowCount());
    mapper->setSeries(series);
    m_timelineChart->addSeries(series);

    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    QStringList l;
    for (int i = 0; i < m_model->rowCount(); ++i) {
        l.push_back(timeAggregationModel()->index(i, 0).data(TimeAggregationModel::DateTimeRole).toDateTime().toString(QStringLiteral("yyyy-MM-dd")));
    }
    xAxis->setCategories(l);
    return m_timelineChart.get();
}
