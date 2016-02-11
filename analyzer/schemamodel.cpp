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

using namespace UserFeedback::Analyzer;

SchemaModel::SchemaModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

SchemaModel::~SchemaModel() = default;

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
    ProductSchemaEntry entry;
    entry.setName(name);
    schema.push_back(entry);
    m_product.setSchema(schema);
    endInsertRows();
}

int SchemaModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int SchemaModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_product.schema().size();
}

QVariant SchemaModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return m_product.schema().at(index.row()).name();
            case 1: return m_product.schema().at(index.row()).type();
        }
    }

    return {};
}

QVariant SchemaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Name");
            case 1: return tr("Type");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags SchemaModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
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
            // TODO
            break;
    }

    m_product.setSchema(schema);

    emit dataChanged(index, index);
    return false;
}
