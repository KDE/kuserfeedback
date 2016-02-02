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

void TimeAggregationModel::reload()
{
    if (!m_sourceModel)
        return;

    QHash<QString, int> m_aggregator;

    for (int i = 0; i < m_sourceModel->rowCount(); ++i) {
        const auto dt = QDateTime::fromString(m_sourceModel->index(i, 0).data().toString(), Qt::ISODate);
        const auto aggr = QString::number(dt.date().year()) + QLatin1Char('-') + QString::number(dt.date().month());
        m_aggregator[aggr]++;
    }

    beginResetModel();
    for (auto it = m_aggregator.constBegin(); it != m_aggregator.constEnd(); ++it) {
        m_data.push_back({ it.key(), it.value() });
    }
    endResetModel();
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
