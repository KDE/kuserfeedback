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

#include <config-userfeedback-version.h>
#include "restclient.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace UserFeedback::Analyzer;

RESTClient::RESTClient(QObject *parent) :
    QObject(parent),
    m_networkAccessManager(new QNetworkAccessManager(this))
{
}

RESTClient::~RESTClient() = default;

void RESTClient::connectToServer(const ServerInfo& info)
{
    m_serverInfo = info;
    if (isConnected())
        emit clientConnected();
}

bool UserFeedback::Analyzer::RESTClient::isConnected() const
{
    return m_serverInfo.url().isValid();
}

QNetworkReply* RESTClient::get(const QString& command)
{
    const auto request = makeRequest(command);
    auto reply = m_networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            emit errorMessage(reply->errorString());
    });
    return reply;
}

QNetworkReply* RESTClient::post(const QString& command, const QByteArray& data)
{
    const auto request = makeRequest(command);
    auto reply = m_networkAccessManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            emit errorMessage(reply->errorString());
    });
    return reply;
}

QNetworkReply* RESTClient::put(const QString &command, const QByteArray &data)
{
    const auto request = makeRequest(command);
    auto reply = m_networkAccessManager->put(request, data);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            emit errorMessage(reply->errorString());
    });
    return reply;
}

QNetworkReply* RESTClient::deleteResource(const QString& command)
{
    const auto request = makeRequest(command);
    auto reply = m_networkAccessManager->deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            emit errorMessage(reply->errorString());
    });
    return reply;
}

QNetworkRequest RESTClient::makeRequest(const QString& command)
{
    Q_ASSERT(isConnected());
    auto url = m_serverInfo.url();
    url.setPath(url.path() + QStringLiteral("/analytics/") + command);
    QNetworkRequest request(url);
    const auto authToken = m_serverInfo.userName().toUtf8() + ':' + m_serverInfo.password().toUtf8();
    request.setRawHeader("Authorization", "Basic " + authToken.toBase64());
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("UserFeedbackAnalyzer/") + QStringLiteral(USERFEEDBACK_VERSION));
    return request;
}
