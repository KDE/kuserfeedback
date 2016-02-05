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

#include "timeaggregationmodel.h"

#include <QDebug>
#include <QDateTime>

#include <algorithm>

using namespace UserFeedback::Analyzer;

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

void TimeAggregationModel::setAggregationMode(AggregationMode mode)
{
    m_mode = mode;
    reload();
}

void TimeAggregationModel::reload()
{
    if (!m_sourceModel)
        return;

    QHash<QString, int> m_aggregator;

    for (int i = 0; i < m_sourceModel->rowCount(); ++i) {
        const auto dt = m_sourceModel->index(i, 0).data().toDateTime();
        const auto aggr = aggregate(dt);
        m_aggregator[aggr]++;
    }

    beginResetModel();
    m_data.clear();
    for (auto it = m_aggregator.constBegin(); it != m_aggregator.constEnd(); ++it) {
        m_data.push_back({ it.key(), it.value() });
    }
    std::sort(m_data.begin(), m_data.end(), [](const Data &lhs, const Data &rhs) {
        return lhs.timeLabel < rhs.timeLabel;
    });
    endResetModel();
}

QString TimeAggregationModel::aggregate(const QDateTime& dt) const
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

    if (role == Qt::DisplayRole) {
        const auto d = m_data.at(index.row());
        switch (index.column()) {
            case 0: return d.timeLabel;
            case 1: return d.samples;
        }
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
