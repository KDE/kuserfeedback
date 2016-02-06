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

#include "chart.h"
#include "timeaggregationmodel.h"

#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>

#include <QAbstractItemModel>
#include <QApplication>
#include <QDateTime>

#include <numeric>

using namespace UserFeedback::Analyzer;
using namespace QtCharts;

Chart::Chart(QObject *parent) :
    QObject(parent),
    m_chart(new QChart),
    m_xAxis(new QDateTimeAxis(this)),
    m_yAxis(new QValueAxis(this))
{
    m_chart->setTheme(qApp->palette().color(QPalette::Window).lightnessF() < 0.25 ? QChart::ChartThemeDark : QChart::ChartThemeLight);

    m_xAxis->setFormat(QStringLiteral("yyyy-MM-dd")); // TODO, follow aggregation mode
    m_chart->addAxis(m_xAxis, Qt::AlignBottom);
    m_yAxis->setTickCount(5);
    m_yAxis->setMinorTickCount(4);
    m_chart->addAxis(m_yAxis, Qt::AlignLeft);
}

Chart::~Chart() = default;

QChart* Chart::chart() const
{
    return m_chart.get();
}

void Chart::setModel(QAbstractItemModel* model)
{
    if (m_model)
        disconnect(m_model, &QAbstractItemModel::modelReset, this, &Chart::modelReset);

    if (!model)
        return;

    m_model = model;
    connect(m_model, &QAbstractItemModel::modelReset, this, &Chart::modelReset);
    modelReset();
}

void Chart::modelReset()
{
    Q_ASSERT(m_model);
    m_chart->removeAllSeries();

    auto series = new QLineSeries(this);
    series->setName(m_model->headerData(1, Qt::Horizontal).toString());

    auto mapper = new QVXYModelMapper(series);
    mapper->setModel(m_model);
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setFirstRow(0);
    mapper->setSeries(series);
    m_chart->addSeries(series);

    // auto-scale axes
    if (const auto rcount = m_model->rowCount()) {
        const auto beginDt = m_model->index(0, 0).data(TimeAggregationModel::DateTimeRole).toDateTime();
        const auto endDt = m_model->index(rcount - 1, 0).data(TimeAggregationModel::DateTimeRole).toDateTime();
        m_xAxis->setRange(beginDt, endDt);
        m_xAxis->setTickCount(std::min(rcount, 12));

        const auto max = m_model->index(0, 0).data(TimeAggregationModel::MaximumValueRole).toInt();
        m_yAxis->setRange(0, max);
        m_yAxis->applyNiceNumbers();
    }

    series->attachAxis(m_xAxis);
    series->attachAxis(m_yAxis);
}
