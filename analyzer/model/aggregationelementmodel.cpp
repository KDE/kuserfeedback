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

#include "aggregationelementmodel.h"

#include <core/product.h>

using namespace UserFeedback::Analyzer;

AggregationElementModel::AggregationElementModel(QObject* parent) :
    QAbstractListModel(parent)
{
}

AggregationElementModel::~AggregationElementModel() = default;

void AggregationElementModel::setProduct(const Product& product)
{
    beginResetModel();
    m_elements.clear();
    for (const auto &entry : product.schema()) {
        for (const auto &elem : entry.elements()) {
            AggregationElement e;
            e.setSchemaEntry(entry);
            e.setSchemaEntryElement(elem);
            e.setType(AggregationElement::Value);
            m_elements.push_back(e);
        }
        if (entry.dataType() != SchemaEntry::Scalar) {
            AggregationElement e;
            e.setSchemaEntry(entry);
            e.setType(AggregationElement::Size);
            m_elements.push_back(e);
        }
    }
    endResetModel();
}

int AggregationElementModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_elements.size();
}

QVariant AggregationElementModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || m_elements.isEmpty())
        return {};

    const auto e = m_elements.at(index.row());
    switch (role) {
        case Qt::DisplayRole: return e.displayString();
        case Qt::EditRole: return QVariant::fromValue(e);
    }

    return {};
}

QModelIndexList AggregationElementModel::match(const QModelIndex& start, int role, const QVariant& value, int hits, Qt::MatchFlags flags) const
{
    if (role == Qt::EditRole && value.userType() == qMetaTypeId<AggregationElement>() && hits == 1 && start.row() == 0) {
        const auto cmp = value.value<AggregationElement>();
        for (int i = 0; i < m_elements.size(); ++i) {
            if (m_elements.at(i) == cmp)
                return { index(i, 0) };
        }
    }
    return QAbstractListModel::match(start, role, value, hits, flags);
}
