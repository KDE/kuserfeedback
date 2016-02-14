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

#include "ratioset.h"
#include "restclient.h"
#include "sample.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#include <algorithm>

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

void DataModel::setProduct(const Product& product)
{
    m_product = product;
    reload();
}

void UserFeedback::Analyzer::DataModel::reload()
{
    if (!m_restClient || !m_restClient->isConnected() || !m_product.isValid())
        return;

    auto reply = m_restClient->get(QStringLiteral("data/") + m_product.name());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            beginResetModel();
            m_data = Sample::fromJson(reply->readAll(), m_product);
            std::sort(m_data.begin(), m_data.end(), [](const Sample &lhs, const Sample &rhs) {
                return lhs.timestamp() < rhs.timestamp();
            });
            endResetModel();
        }
    });
}

int DataModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_product.schema().size() + 1;
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
        const auto sample = m_data.at(index.row());
        if (index.column() == 0)
            return sample.timestamp();
        const auto v = sample.value(m_product.schema().at(index.column() - 1).name());
        if (v.type() == QVariant::StringList)
            return v.toStringList().join(QStringLiteral(", "));
        if (v.userType() == qMetaTypeId<RatioSet>())
            return v.value<RatioSet>().displayString();
        return v;
    } else if (role == SampleRole) {
        return QVariant::fromValue(m_data.at(index.row()));
    } else if (role == AllSamplesRole) {
        return QVariant::fromValue(m_data);
    }

    return {};
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && m_product.isValid()) {
        if (section == 0)
            return tr("Timestamp");
        if (section - 1 < m_product.schema().size())
            return m_product.schema().at(section - 1).name();
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
