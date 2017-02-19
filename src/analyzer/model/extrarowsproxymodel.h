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

#ifndef USERFEEDBACK_ANALYZER_EXTRAROWSPROXYMODEL_H
#define USERFEEDBACK_ANALYZER_EXTRAROWSPROXYMODEL_H

#include <QIdentityProxyModel>

namespace UserFeedback {
namespace Analyzer {

class ExtraRowsProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    explicit ExtraRowsProxyModel(QObject *parent = nullptr);
    ~ExtraRowsProxyModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex & child) const override;

protected:
    virtual QVariant extraData(int row, int column, int role) const;

private:
    int m_extraRowCount = 1;
};

}}

#endif // USERFEEDBACK_ANALYZER_EXTRAROWSPROXYMODEL_H
