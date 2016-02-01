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

#include "datamodel.h"

#include "restclient.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace UserFeedback::Analyzer;

DataModel::DataModel(QObject *parent) : QAbstractTableModel(parent)
{
}

DataModel::~DataModel() = default;

void DataModel::setRESTClient(RESTClient* client)
{
    Q_ASSERT(client);
    m_restClient = client;
    connect(client, &RESTClient::clientConnected, this, &DataModel::reload);
    if (client->isConnected())
        reload();
}

void DataModel::setProductId(const QString& productId)
{
    m_productId = productId;
    reload();
}

void UserFeedback::Analyzer::DataModel::reload()
{
    if (!m_restClient || !m_restClient->isConnected() || m_productId.isEmpty())
        return;

    auto reply = m_restClient->get(QStringLiteral("data/") + m_productId);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            beginResetModel();
            m_data = QJsonDocument::fromJson(reply->readAll()).array();
            endResetModel();
        }
    });
}

int DataModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 5;
}

int DataModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (role == Qt::DisplayRole) {
        const auto obj = m_data.at(index.row()).toObject();
        switch (index.column()) {
            case 0: return obj.value(QStringLiteral("timestamp")).toString();
            case 1: return obj.value(QStringLiteral("version")).toString();
            case 2: return obj.value(QStringLiteral("platform")).toString();
            case 3: return obj.value(QStringLiteral("startCount")).toString();
            case 4: return obj.value(QStringLiteral("usageTime")).toString();
        }
    }

    return {};
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
            case 0: return tr("Timestamp");
            case 1: return tr("Version");
            case 2: return tr("Platform");
            case 3: return tr("Starts");
            case 4: return tr("Used Time");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
