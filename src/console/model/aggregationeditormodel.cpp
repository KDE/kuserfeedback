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

#include "aggregationeditormodel.h"

#include <core/aggregation.h>
#include <core/util.h>

using namespace UserFeedback::Console;

AggregationEditorModel::AggregationEditorModel(QObject* parent) :
    QAbstractTableModel(parent)
{
}

AggregationEditorModel::~AggregationEditorModel() = default;

Product AggregationEditorModel::product() const
{
    return m_product;
}

void AggregationEditorModel::setProduct(const Product& product)
{
    beginResetModel();
    m_product = product;
    endResetModel();
}

int AggregationEditorModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int AggregationEditorModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_product.aggregations().size();
}

QVariant AggregationEditorModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !m_product.isValid())
        return {};

    if (role == Qt::DisplayRole) {
        const auto aggr = m_product.aggregations().at(index.row());
        switch (index.column()) {
            case 0:
                return Util::enumToString(aggr.type());
            case 1:
                if (aggr.elements().isEmpty())
                    return tr("<none>");
                return aggr.elements().at(0).displayString();
        }
    } else if (role == Qt::EditRole) {
        const auto aggr = m_product.aggregations().at(index.row());
        switch (index.column()) {
            case 0:
                return QVariant::fromValue(aggr.type());
            case 1:
                if (aggr.elements().isEmpty())
                    return QVariant::fromValue(AggregationElement());
                return QVariant::fromValue(aggr.elements().at(0));
        }
    }

    return {};
}

QVariant AggregationEditorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Type");
            case 1: return tr("Element");
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags AggregationEditorModel::flags(const QModelIndex& index) const
{
    const auto baseFlags = QAbstractTableModel::flags(index);
    if (index.isValid())
        return baseFlags | Qt::ItemIsEditable;

    return baseFlags;
}

bool AggregationEditorModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    auto aggrs = m_product.aggregations();
    auto &aggr = aggrs[index.row()];
    switch (index.column()) {
        case 0:
            aggr.setType(value.value<Aggregation::Type>());
            break;
        case 1:
            aggr.setElements({ value.value<AggregationElement>() });
            break;
    }
    m_product.setAggregations(aggrs);
    emit dataChanged(index, index);
    return true;
}
