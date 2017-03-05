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

#ifndef USERFEEDBACK_CONSOLE_RESTCLIENT_H
#define USERFEEDBACK_CONSOLE_RESTCLIENT_H

#include "serverinfo.h"

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

namespace UserFeedback {
namespace Console {

/** REST communication handling. */
class RESTClient : public QObject
{
    Q_OBJECT
public:
    explicit RESTClient(QObject *parent = nullptr);
    ~RESTClient();

    void setServerInfo(const ServerInfo &info);
    void setConnected(bool connected);
    bool isConnected() const;

    QNetworkReply* get(const QString &command);
    QNetworkReply* post(const QString &command, const QByteArray &data);
    QNetworkReply* put(const QString &command, const QByteArray &data);
    QNetworkReply* deleteResource(const QString &command);

signals:
    void clientConnected();
    void errorMessage(const QString &msg);

private:
    QNetworkRequest makeRequest(const QString &command);
    void setupMessageHandler(QNetworkReply *reply);

    ServerInfo m_serverInfo;
    QNetworkAccessManager *m_networkAccessManager;
    bool m_isConnected = false;
};

}
}

#endif // USERFEEDBACK_CONSOLE_RESTCLIENT_H
