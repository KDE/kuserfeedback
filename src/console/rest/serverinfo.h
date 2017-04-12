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

#ifndef USERFEEDBACK_CONSOLE_SERVERINFO_H
#define USERFEEDBACK_CONSOLE_SERVERINFO_H

#include <QSharedDataPointer>

class QUrl;

namespace UserFeedback {
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

    QUrl url() const;
    void setUrl(const QUrl& url);

    QString userName() const;
    void setUserName(const QString& userName);

    QString password() const;
    void setPassword(const QString& password);

    /** Save to settings. */
    void save() const;
    /** Load from settings. */
    static ServerInfo load(const QString &url);

private:
    QSharedDataPointer<ServerInfoData> d;
};

}
}

#endif // USERFEEDBACK_CONSOLE_SERVERINFO_H
