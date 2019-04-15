/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include "handshakejob.h"

#include <rest/restapi.h>
#include <rest/restclient.h>

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

using namespace KUserFeedback::Console;

HandshakeJob::HandshakeJob(RESTClient* restClient, QObject* parent)
    : Job(parent)
    , m_restClient(restClient)
{
    auto reply = RESTApi::checkSchema(restClient);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit info(tr("Connected to %1.").arg(m_restClient->serverInfo().url().toString()));
            processResponse(reply);
        } else {
            emitError(reply->errorString());
        }
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::redirected, this, [this](const auto &url) {
        auto s = m_restClient->serverInfo();
        auto u = url;
        auto p = u.path();
        p.remove(QLatin1String("analytics/check_schema"));
        u.setPath(p);
        s.setUrl(u);
        m_restClient->setServerInfo(s);
    });
}

HandshakeJob::~HandshakeJob()
{
}

void HandshakeJob::processResponse(QNetworkReply* reply)
{
    const auto doc = QJsonDocument::fromJson(reply->readAll());
    const auto obj = doc.object();

    const auto protoVer = obj.value(QLatin1String("protocolVersion")).toInt();
    if (protoVer != 2) {
        emitError(tr("Incompatible protocol: %1.").arg(protoVer));
        return;
    }

    const auto prevSchema = obj.value(QLatin1String("previousSchemaVersion")).toInt();
    const auto curSchema = obj.value(QLatin1String("currentSchemaVersion")).toInt();
    if (prevSchema != curSchema)
        emit info(tr("Updated database schema from version %1 to %2.").arg(prevSchema).arg(curSchema));

    m_restClient->setConnected(true);
    emitFinished();
}
