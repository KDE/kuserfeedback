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

using namespace UserFeedback::Console;

RESTClient::RESTClient(QObject *parent) :
    QObject(parent),
    m_networkAccessManager(new QNetworkAccessManager(this))
{
}

RESTClient::~RESTClient() = default;

ServerInfo RESTClient::serverInfo() const
{
    return m_serverInfo;
}

void RESTClient::setServerInfo(const ServerInfo& info)
{
    m_isConnected = false;
    m_serverInfo = info;
}

void RESTClient::setConnected(bool connected)
{
    if (m_isConnected == connected)
        return;
    m_isConnected = connected;
    if (isConnected())
        emit clientConnected();
}

bool RESTClient::isConnected() const
{
    return m_serverInfo.url().isValid() && m_isConnected;
}

QNetworkReply* RESTClient::get(const QString& command)
{
    const auto request = makeRequest(command);
    auto reply = m_networkAccessManager->get(request);
    setupMessageHandler(reply);
    return reply;
}

QNetworkReply* RESTClient::post(const QString& command, const QByteArray& data)
{
    auto request = makeRequest(command);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    auto reply = m_networkAccessManager->post(request, data);
    setupMessageHandler(reply);
    return reply;
}

QNetworkReply* RESTClient::put(const QString &command, const QByteArray &data)
{
    const auto request = makeRequest(command);
    auto reply = m_networkAccessManager->put(request, data);
    setupMessageHandler(reply);
    return reply;
}

QNetworkReply* RESTClient::deleteResource(const QString& command)
{
    const auto request = makeRequest(command);
    auto reply = m_networkAccessManager->deleteResource(request);
    setupMessageHandler(reply);
    return reply;
}

QNetworkRequest RESTClient::makeRequest(const QString& command)
{
    Q_ASSERT(m_serverInfo.url().isValid());
    auto url = m_serverInfo.url();
    auto path = url.path();
    if (!path.endsWith(QLatin1Char('/')))
        path += QLatin1Char('/');
    path +=  QStringLiteral("analytics/") + command;
    url.setPath(path);
    QNetworkRequest request(url);
    const auto authToken = m_serverInfo.userName().toUtf8() + ':' + m_serverInfo.password().toUtf8();
    request.setRawHeader("Authorization", "Basic " + authToken.toBase64());
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("UserFeedbackConsole/") + QStringLiteral(USERFEEDBACK_VERSION));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    return request;
}

void RESTClient::setupMessageHandler(QNetworkReply* reply)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError)
            return;
        emit errorMessage(reply->errorString());
        const auto d = reply->readAll();
        if (!d.isEmpty())
            emit errorMessage(QString::fromUtf8(d));
    });
}
