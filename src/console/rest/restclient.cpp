/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include <kuserfeedback_version.h>
#include "restclient.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KUserFeedback::Console;

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

QNetworkAccessManager* RESTClient::networkAccessManager() const
{
    return m_networkAccessManager;
}

QNetworkRequest RESTClient::makeRequest(const QString& command)
{
    Q_ASSERT(m_serverInfo.url().isValid());
    auto url = m_serverInfo.url();
    auto path = url.path();
    if (!path.endsWith(QLatin1Char('/')))
        path += QLatin1Char('/');
    path += command;
    url.setPath(path);
    QNetworkRequest request(url);
    const QByteArray authToken = m_serverInfo.userName().toUtf8() + ':' + m_serverInfo.password().toUtf8();
    request.setRawHeader("Authorization", QByteArray("Basic " + authToken.toBase64()));
    request.setHeader(QNetworkRequest::UserAgentHeader, QString(QStringLiteral("UserFeedbackConsole/") + QStringLiteral(KUSERFEEDBACK_VERSION_STRING)));
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
