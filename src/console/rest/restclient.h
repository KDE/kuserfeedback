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

#ifndef KUSERFEEDBACK_CONSOLE_RESTCLIENT_H
#define KUSERFEEDBACK_CONSOLE_RESTCLIENT_H

#include "serverinfo.h"

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

namespace KUserFeedback {
namespace Console {

/** REST communication handling. */
class RESTClient : public QObject
{
    Q_OBJECT
public:
    explicit RESTClient(QObject *parent = nullptr);
    ~RESTClient();

    ServerInfo serverInfo() const;
    void setServerInfo(const ServerInfo &info);
    void setConnected(bool connected);
    bool isConnected() const;

    QNetworkReply* get(const QString &command);
    QNetworkReply* post(const QString &command, const QByteArray &data);
    QNetworkReply* put(const QString &command, const QByteArray &data);
    QNetworkReply* deleteResource(const QString &command);

    QNetworkAccessManager* networkAccessManager() const;
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

#endif // KUSERFEEDBACK_CONSOLE_RESTCLIENT_H
