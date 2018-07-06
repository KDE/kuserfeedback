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

#include "test-config.h"

#include "servercontroller.h"

#include <rest/restapi.h>
#include <rest/restclient.h>

#include <QNetworkReply>
#include <QUrl>
#include <QSignalSpy>

using namespace KUserFeedback::Console;

ServerController::ServerController()
{
    m_process.setProgram(QStringLiteral(PHP_EXECUTABLE));
    m_process.setWorkingDirectory(QLatin1String(CURRENT_SOURCE_DIR "/../src/testserver"));
    m_process.setArguments({QStringLiteral("-S"), QStringLiteral("localhost:1984"), QStringLiteral("router.php")});
    m_process.setProcessChannelMode(QProcess::ForwardedChannels);
}

ServerController::~ServerController()
{
    if (m_process.state() == QProcess::Running)
        stop();
}

bool ServerController::start()
{
    m_process.start();
    if (!m_process.waitForStarted(1000)) {
        qWarning() << "Failed to launch:" << m_process.program() << m_process.arguments() << m_process.errorString();
        return false;
    }

    // wait for the server to become available, otherwise the HTTP access below fails
    m_process.waitForReadyRead(100);

    ServerInfo s;
    s.setUrl(url());
    RESTClient client;
    client.setServerInfo(s);
    auto reply = RESTApi::checkSchema(&client);

    QSignalSpy spy(reply, &QNetworkReply::finished);
    if (!spy.wait())
        return false;

    if (reply->error() != QNetworkReply::NoError)
        qWarning() << "Schema check failed:" << reply->errorString();
    return reply->error() == QNetworkReply::NoError;
}

void ServerController::stop()
{
    m_process.terminate();
    m_process.waitForFinished(1000);
}

QUrl ServerController::url() const
{
    return QUrl(QStringLiteral("http://localhost:1984"));
}
