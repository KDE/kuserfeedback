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

#include "aggregateddatamodel.h"
#include <model/timeaggregationmodel.h>

#include <QDebug>

using namespace UserFeedback::Console;

AggregatedDataModel::AggregatedDataModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

AggregatedDataModel::~AggregatedDataModel() = default;

void AggregatedDataModel::addSourceModel(QAbstractItemModel* model, const QString &prefix)
{
    Q_ASSERT(model);
    m_models.push_back(model);
    m_prefixes.push_back(prefix);
    connect(model, &QAbstractItemModel::modelReset, this, &AggregatedDataModel::recreateColumnMapping);
    recreateColumnMapping();
}

void AggregatedDataModel::clear()
{
    beginResetModel();
    m_columnMapping.clear();
    m_columnOffset.clear();
    foreach (auto model, m_models)
        disconnect(model, &QAbstractItemModel::modelReset, this, &AggregatedDataModel::recreateColumnMapping);
    m_models.clear();
    m_prefixes.clear();
    endResetModel();
}

int AggregatedDataModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_columnMapping.size();
}

int AggregatedDataModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid() || m_models.isEmpty())
        return 0;
    return m_models.first()->rowCount();
}

QVariant AggregatedDataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || m_models.isEmpty())
        return {};

    if (index.column() == 0 && role == Qt::DisplayRole) {
        return m_models.at(0)->index(index.row(), 0).data(TimeAggregationModel::TimeDisplayRole);
    }

    const auto model = m_models.at(m_columnMapping.at(index.column()));
    const auto srcIdx = model->index(index.row(), m_columnOffset.at(index.column()));
    if (role == Qt::DisplayRole)
        return srcIdx.data(TimeAggregationModel::DataDisplayRole);
    return srcIdx.data(role);
}

QVariant AggregatedDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && !m_models.isEmpty()) {
        const auto v = m_models.at(m_columnMapping.at(section))->headerData(m_columnOffset.at(section), orientation, role);
        if (role != Qt::DisplayRole || m_prefixes.at(m_columnMapping.at(section)).isEmpty())
            return v;
        return QString(m_prefixes.at(m_columnMapping.at(section)) + QStringLiteral(": ") + v.toString());
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void AggregatedDataModel::recreateColumnMapping()
{
    beginResetModel();
    m_columnMapping.clear();
    m_columnOffset.clear();

    for (int i = 0; i < m_models.size(); ++i) {
        for (int j = (i == 0 ? 0 : 1); j < m_models.at(i)->columnCount(); ++j) {
            m_columnMapping.push_back(i);
            m_columnOffset.push_back(j);
        }
    }
    endResetModel();
}
