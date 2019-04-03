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

#ifndef KUSERFEEDBACK_CONSOLE_SERVERINFO_H
#define KUSERFEEDBACK_CONSOLE_SERVERINFO_H

#include <QSharedDataPointer>

class QStringList;
class QUrl;

namespace KUserFeedback {
namespace Console {

class ServerInfoData;

/** Connection information for an analytics server. */
class ServerInfo
{
public:
    ServerInfo();
    ServerInfo(const ServerInfo &other);
    ~ServerInfo();
    ServerInfo& operator=(const ServerInfo &other);

    bool isValid() const;

    QString name() const;
    void setName(const QString &name);

    QUrl url() const;
    void setUrl(const QUrl& url);

    QString userName() const;
    void setUserName(const QString& userName);

    QString password() const;
    void setPassword(const QString& password);

    /** Save to settings. */
    void save() const;
    /** Load from settings. */
    static ServerInfo load(const QString &name);
    /** Delete stored information for the server with name @p name. */
    static void remove(const QString &name);

    /** Returns the names of all configured servers. */
    static QStringList allServerInfoNames();

private:
    QSharedDataPointer<ServerInfoData> d;
};

}
}

#endif // KUSERFEEDBACK_CONSOLE_SERVERINFO_H
