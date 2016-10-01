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

#include "schemamodel.h"

#include <core/schemaentryelement.h>

#include <limits>

using namespace UserFeedback::Analyzer;

static const auto TOPLEVEL = std::numeric_limits<quintptr>::max();

SchemaModel::SchemaModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

SchemaModel::~SchemaModel() = default;

Product SchemaModel::product() const
{
    return m_product;
}

void SchemaModel::setProduct(const Product &product)
{
    beginResetModel();
    m_product = product;
    endResetModel();
}

void SchemaModel::addEntry(const QString &name)
{
    auto schema = m_product.schema();
    beginInsertRows(QModelIndex(), schema.size(), schema.size());
    SchemaEntry entry;
    entry.setName(name);
    schema.push_back(entry);
    m_product.setSchema(schema);
    endInsertRows();
}

void SchemaModel::deleteEntry(int row)
{
    auto schema = m_product.schema();
    beginRemoveRows(QModelIndex(), row, row);
    schema.removeAt(row);
    m_product.setSchema(schema);
    endRemoveRows();
}

int SchemaModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

int SchemaModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return m_product.schema().size();
    if (parent.internalId() == TOPLEVEL)
        return m_product.schema().at(parent.row()).elements().size();
    return 0;
}

QVariant SchemaModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.internalId() != TOPLEVEL)
        return {};

    switch (index.column()) {
        case 0:
            if (role == Qt::DisplayRole || role == Qt::EditRole)
                return m_product.schema().at(index.row()).name();
            break;
        case 1:
            if (role == Qt::DisplayRole)
                return SchemaEntry::displayString(m_product.schema().at(index.row()).type());
            if (role == Qt::EditRole)
                return QVariant::fromValue(m_product.schema().at(index.row()).type());
            break;
        case 2:
            if (role == Qt::DisplayRole)
                return m_product.schema().at(index.row()).aggregationType();
    }

    return {};
}

QVariant SchemaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Name");
            case 1: return tr("Type");
            case 2: return tr("Aggregation");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags SchemaModel::flags(const QModelIndex &index) const
{
    const auto baseFlags = QAbstractItemModel::flags(index);
    if (index.isValid())
        return baseFlags | Qt::ItemIsEditable;
    return baseFlags;
}

bool SchemaModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    auto schema = m_product.schema();
    auto &entry = schema[index.row()];

    switch (index.column()) {
        case 0:
            entry.setName(value.toString());
            break;
        case 1:
            entry.setType(value.value<SchemaEntry::Type>());
            break;
    }

    m_product.setSchema(schema);

    emit dataChanged(index, index);
    return false;
}

QModelIndex SchemaModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};
    if (!parent.isValid())
        return createIndex(row, column, TOPLEVEL);
    if (parent.internalId() == TOPLEVEL)
        return createIndex(row, column, parent.row());
    return {};
}

QModelIndex SchemaModel::parent(const QModelIndex &index) const
{
    if (!index.isValid() || index.internalId() == TOPLEVEL)
        return {};
    return createIndex(index.internalId(), 0);
}
