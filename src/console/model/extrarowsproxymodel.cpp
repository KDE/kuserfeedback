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

#include "extrarowsproxymodel.h"

using namespace KUserFeedback::Console;

ExtraRowsProxyModel::ExtraRowsProxyModel(QObject* parent) :
    QIdentityProxyModel(parent)
{
}

ExtraRowsProxyModel::~ExtraRowsProxyModel() = default;

int ExtraRowsProxyModel::rowCount(const QModelIndex& parent) const
{
    if (!sourceModel())
        return 0;
    if (parent.isValid())
        return sourceModel()->rowCount(mapToSource(parent));
    return sourceModel()->rowCount() + m_extraRowCount;
}

QVariant ExtraRowsProxyModel::data(const QModelIndex& index, int role) const
{
    if (!sourceModel())
        return {};
    if (!index.isValid() || index.parent().isValid() || index.row() < sourceModel()->rowCount())
        return QIdentityProxyModel::data(index, role);
    return extraData(index.row() - sourceModel()->rowCount(), index.column(), role);
}

QModelIndex ExtraRowsProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!sourceModel())
        return {};
    if (parent.isValid() || row < sourceModel()->rowCount())
        return QIdentityProxyModel::index(row, column, parent);
    if (row >= rowCount())
        return {};
    return createIndex(row, column);
}

QModelIndex ExtraRowsProxyModel::parent(const QModelIndex& child) const
{
    if (!sourceModel())
        return {};
    if (child.internalId() == 0 && child.row() >= sourceModel()->rowCount())
        return {};
    return QIdentityProxyModel::parent(child);
}


QVariant ExtraRowsProxyModel::extraData(int row, int column, int role) const
{
    // TODO split into sperate model!
    Q_UNUSED(row);
    if (role == Qt::DisplayRole)
        return headerData(column, Qt::Horizontal, Qt::DisplayRole);
    return {};
}
