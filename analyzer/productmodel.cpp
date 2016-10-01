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

#include "productmodel.h"

#include <rest/restapi.h>
#include <rest/restclient.h>

#include <QNetworkReply>

using namespace UserFeedback::Analyzer;

ProductModel::ProductModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

ProductModel::~ProductModel() = default;

void ProductModel::setRESTClient(RESTClient* client)
{
    m_restClient = client;
    connect(m_restClient, &RESTClient::clientConnected, this, &ProductModel::reload);
}

void ProductModel::reload()
{
    qDebug() << Q_FUNC_INFO;
    Q_ASSERT(m_restClient);
    if (!m_restClient->isConnected())
        return;

    auto reply = RESTApi::getProducts(m_restClient);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            beginResetModel();
            auto json = reply->readAll();
            m_products = Product::fromJson(json);
            endResetModel();
        }
    });
}

int ProductModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_products.size();
}

QVariant ProductModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    switch (role) {
        case Qt::DisplayRole:
            return m_products.at(index.row()).name();
        case ProductRole:
            return QVariant::fromValue(m_products.at(index.row()));
    }
    return {};
}

QVariant ProductModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Products");
        }
    }
    return QAbstractListModel::headerData(section, orientation, role);
}
