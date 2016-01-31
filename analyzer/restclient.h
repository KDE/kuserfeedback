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

#ifndef USERFEEDBACK_ANALYZER_RESTCLIENT_H
#define USERFEEDBACK_ANALYZER_RESTCLIENT_H

#include "serverinfo.h"

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

namespace UserFeedback {
namespace Analyzer {

/** REST communication handling. */
class RESTClient : public QObject
{
    Q_OBJECT
public:
    explicit RESTClient(QObject *parent = nullptr);
    ~RESTClient();

    void connectToServer(const ServerInfo &info);
    bool isConnected() const;

    QNetworkReply* get(const QString &command);
    QNetworkReply* post(const QString &command, const QByteArray &data);
    QNetworkReply* deleteResource(const QString &command);

signals:
    void clientConnected();

private:
    ServerInfo m_serverInfo;
    QNetworkAccessManager *m_networkAccessManager;
};

}
}

#endif // USERFEEDBACK_ANALYZER_RESTCLIENT_H
