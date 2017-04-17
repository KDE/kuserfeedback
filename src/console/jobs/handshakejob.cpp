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

#include "handshakejob.h"

#include <rest/restapi.h>
#include <rest/restclient.h>

#include <QDebug>
#include <QNetworkReply>

using namespace UserFeedback::Console;

HandshakeJob::HandshakeJob(RESTClient* restClient, QObject* parent)
    : Job(parent)
    , m_restClient(restClient)
{
    auto reply = RESTApi::checkSchema(restClient);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit info(tr("Connected to %1.").arg(m_restClient->serverInfo().url().toString()));
            emit info(QString::fromUtf8(reply->readAll()));
            m_restClient->setConnected(true);
            emitFinished();
        } else {
            emitError(reply->errorString());
        }
    });
    connect(reply, &QNetworkReply::redirected, this, [this, reply](const auto &url) {
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
