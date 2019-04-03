/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include "timeaggregationmodel.h"
#include <core/sample.h>
#include <model/datamodel.h>

#include <QDebug>

#include <algorithm>
#include <numeric>

using namespace KUserFeedback::Console;

TimeAggregationModel::TimeAggregationModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

TimeAggregationModel::~TimeAggregationModel() = default;

void TimeAggregationModel::setSourceModel(QAbstractItemModel* model)
{
    Q_ASSERT(model);
    m_sourceModel = model;
    connect(model, &QAbstractItemModel::modelReset, this, [this]() { reload(); });
    reload();
}

TimeAggregationModel::AggregationMode TimeAggregationModel::aggregationMode() const
{
    return m_mode;
}

void TimeAggregationModel::setAggregationMode(AggregationMode mode)
{
    m_mode = mode;
    reload();
}

void TimeAggregationModel::reload()
{
    if (!m_sourceModel)
        return;

    // TODO use the fact that the source model is sorted by time!
    QHash<QDateTime, QVector<Sample>> aggregator;
    for (int i = 0; i < m_sourceModel->rowCount(); ++i) {
        const auto dt = m_sourceModel->index(i, 0).data().toDateTime();
        const auto aggr = aggregate(dt);
        aggregator[aggr].push_back(m_sourceModel->index(i, 0).data(DataModel::SampleRole).value<Sample>());
    }

    beginResetModel();
    m_data.clear();
    m_maxValue = 0;
    for (auto it = aggregator.constBegin(); it != aggregator.constEnd(); ++it) {
        m_data.push_back({ it.key(), it.value() });
        m_maxValue = std::max(m_maxValue, it.value().size());
    }
    std::sort(m_data.begin(), m_data.end(), [](const Data &lhs, const Data &rhs) {
        return lhs.time < rhs.time;
    });
    endResetModel();
}

QDateTime TimeAggregationModel::aggregate(const QDateTime &dt) const
{
    switch (m_mode) {
        case AggregateYear:
            return QDateTime(QDate(dt.date().year(), 1, 1));
        case AggregateMonth:
            return QDateTime(QDate(dt.date().year(), dt.date().month(), 1));
        case AggregateWeek:
        {
            auto d = dt.date();
            d = d.addDays(1 - d.dayOfWeek());
            return QDateTime(d);
        }
        case AggregateDay:
            return QDateTime(dt.date());
    }

    Q_UNREACHABLE();
}

QString TimeAggregationModel::timeToString(const QDateTime &dt) const
{
    switch (m_mode) {
        case AggregateYear:
            return QString::number(dt.date().year());
        case AggregateMonth:
            return dt.date().toString(QStringLiteral("yyyy-MM"));
        case AggregateWeek:
        {
            int year = 0;
            int week = dt.date().weekNumber(&year);
            return QString::number(year) + QStringLiteral("-w") + QString::number(week);
        }
        case AggregateDay:
            return dt.date().toString(Qt::ISODate);
    }

    Q_UNREACHABLE();
}

int TimeAggregationModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int TimeAggregationModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

QVariant TimeAggregationModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    if (role == Qt::DisplayRole || role == DataDisplayRole || role == AccumulatedDisplayRole) {
        const auto d = m_data.at(index.row());
        switch (index.column()) {
            case 0: return d.time.toMSecsSinceEpoch(); // this is required by QtCharts...
            case 1: return d.samples.size();
        }
    } else if (role == DateTimeRole) {
        return m_data.at(index.row()).time;
    } else if (role == TimeDisplayRole) {
        return timeToString(m_data.at(index.row()).time);
    } else if (role == MaximumValueRole) {
        return m_maxValue;
    } else if (role == SamplesRole) {
        return QVariant::fromValue(m_data.at(index.row()).samples);
    } else if (role == AllSamplesRole) {
        return m_sourceModel->index(0, 0).data(DataModel::AllSamplesRole);
    }

    return {};
}

QVariant TimeAggregationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Time");
            case 1: return tr("Samples");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
