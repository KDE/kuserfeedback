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

#include "surveymodel.h"

#include "restclient.h"

#include <QDebug>
#include <QNetworkReply>
#include <QUrl>

using namespace UserFeedback::Analyzer;

SurveyModel::SurveyModel(QObject *parent) : QAbstractTableModel(parent)
{
}

SurveyModel::~SurveyModel() = default;

void SurveyModel::setRESTClient(RESTClient* client)
{
    Q_ASSERT(client);
    m_restClient = client;
    connect(client, &RESTClient::clientConnected, this, &SurveyModel::reload);
    reload();
}

void SurveyModel::setProductId(const QString& product)
{
    m_productId = product;
    reload();
}

void SurveyModel::reload()
{
    if (!m_restClient || !m_restClient->isConnected() || m_productId.isEmpty())
        return;

    auto reply = m_restClient->get(QStringLiteral("surveys/") + m_productId);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            beginResetModel();
            const auto data = reply->readAll();
            m_surveys = Survey::fromJson(data);
            endResetModel();
        }
    });
}

int SurveyModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int SurveyModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_surveys.size();
}

QVariant SurveyModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    if (role == Qt::DisplayRole) {
        const auto survey = m_surveys.at(index.row());
        switch (index.column()) {
            case 0: return survey.name();
            case 1: return survey.url().toString();
        }
    } else if (role == SurveyRole) {
        return QVariant::fromValue(m_surveys.at(index.row()));
    }
    return {};
}

QVariant SurveyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Name");
            case 1: return tr("URL");
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
