/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#ifndef USERFEEDBACK_CONSOLE_SINGLEROWFILTERPROXYMODEL_H
#define USERFEEDBACK_CONSOLE_SINGLEROWFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace UserFeedback {
namespace Console {

/** Keep a single row from the source model. */
class SingleRowFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SingleRowFilterProxyModel(QObject *parent = nullptr);
    ~SingleRowFilterProxyModel();

    void setRow(int row);
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override;

private:
    int m_row = 0;
};

}}

#endif // USERFEEDBACK_CONSOLE_SINGLEROWFILTERPROXYMODEL_H
