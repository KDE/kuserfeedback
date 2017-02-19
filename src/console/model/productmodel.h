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

#ifndef USERFEEDBACK_CONSOLE_PRODUCTMODEL_H
#define USERFEEDBACK_CONSOLE_PRODUCTMODEL_H

#include <core/product.h>
#include <rest/serverinfo.h>

#include <QAbstractListModel>
#include <QVector>

namespace UserFeedback {
namespace Console {

class RESTClient;

/** Self-updating product model. */
class ProductModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ProductModel(QObject *parent = nullptr);
    ~ProductModel();

    enum Roles {
        ProductRole = Qt::UserRole + 1
    };

    void setRESTClient(RESTClient *client);
    void clear();
    void reload();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    void mergeProducts(QVector<Product> &&products);

    RESTClient *m_restClient = nullptr;
    QVector<Product> m_products;
};
}
}

#endif // USERFEEDBACK_CONSOLE_PRODUCTMODEL_H
