/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include "aggregationelementeditmodel.h"

using namespace KUserFeedback::Console;

AggregationElementEditModel::AggregationElementEditModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

AggregationElementEditModel::~AggregationElementEditModel() = default;

Aggregation AggregationElementEditModel::aggregation() const
{
    return m_aggr;
}

void AggregationElementEditModel::setAggregation(const Aggregation& aggregation)
{
    beginResetModel();
    m_aggr = aggregation;
    endResetModel();
}

int AggregationElementEditModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_aggr.elements().size();
}

QVariant AggregationElementEditModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    switch (role) {
        case Qt::DisplayRole:
        {
            const auto elem = m_aggr.elements().at(index.row());
            if (elem.isValid())
                return elem.displayString();
            return tr("<new element>");
        }
        case Qt::EditRole:
            return QVariant::fromValue(m_aggr.elements().at(index.row()));
    }

    return {};
}

QVariant AggregationElementEditModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
        return tr("Element");
    return QAbstractListModel::headerData(section, orientation, role);
}

Qt::ItemFlags AggregationElementEditModel::flags(const QModelIndex& index) const
{
    const auto baseFlags = QAbstractListModel::flags(index);
    return baseFlags | Qt::ItemIsEditable;
}

bool AggregationElementEditModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || value.isNull())
        return false;

    auto elems = m_aggr.elements();
    elems[index.row()] = value.value<AggregationElement>();
    m_aggr.setElements(elems);
    emit dataChanged(index, index);
    return true;
}
