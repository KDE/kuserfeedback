/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#include "singlerowfilterproxymodel.h"

using namespace KUserFeedback::Console;

SingleRowFilterProxyModel::SingleRowFilterProxyModel(QObject* parent) :
    QSortFilterProxyModel(parent)
{
}

SingleRowFilterProxyModel::~SingleRowFilterProxyModel() = default;

void SingleRowFilterProxyModel::setRow(int row)
{
    beginFilterChange();
    m_row = row;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
}

bool SingleRowFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    if (source_parent.isValid())
        return false;
    return source_row == m_row;
}

#include "moc_singlerowfilterproxymodel.cpp"
