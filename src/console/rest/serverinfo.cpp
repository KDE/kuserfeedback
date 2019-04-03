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

#include "serverinfo.h"

#include <QSharedData>
#include <QSettings>
#include <QUrl>

using namespace KUserFeedback::Console;

namespace KUserFeedback {
namespace Console {
class ServerInfoData : public QSharedData
{
public:
    static QString groupName(const QString &name);

    QString name;
    QUrl url;
    QString userName;
    QString password;
};

}
}

QString ServerInfoData::groupName(const QString &name)
{
    return QString::fromLatin1(QUrl::toPercentEncoding(name));
}

ServerInfo::ServerInfo() : d(new ServerInfoData) {}
ServerInfo::ServerInfo(const ServerInfo&) = default;
ServerInfo::~ServerInfo() = default;
ServerInfo& ServerInfo::operator=(const ServerInfo&) = default;

bool ServerInfo::isValid() const
{
    return d->url.isValid();
}

QString ServerInfo::name() const
{
    // ### temporary
    if (d->name.isEmpty())
        return d->url.toString();
    return d->name;
}

void ServerInfo::setName(const QString& name)
{
    d->name = name;
}

QUrl ServerInfo::url() const
{
    return d->url;
}

void ServerInfo::setUrl(const QUrl& url)
{
    d->url = url;
}

QString ServerInfo::userName() const
{
    return d->userName;
}

void ServerInfo::setUserName(const QString& userName)
{
    d->userName = userName;
}

QString ServerInfo::password() const
{
    return d->password;
}

void ServerInfo::setPassword(const QString& password)
{
    d->password = password;
}

void ServerInfo::save() const
{
    QSettings settings(QStringLiteral("KDE"), QStringLiteral("UserFeedbackConsole.servers"));
    settings.beginGroup(ServerInfoData::groupName(name()));
    settings.setValue(QStringLiteral("name"), name());
    settings.setValue(QStringLiteral("url"), url().toString());
    settings.setValue(QStringLiteral("userName"), userName());
    settings.setValue(QStringLiteral("password"), password()); // TODO
    settings.endGroup();

    auto allServers = settings.value(QStringLiteral("ServerNames")).toStringList();
    if (!allServers.contains(name()))
        allServers.push_back(name());
    settings.setValue(QStringLiteral("ServerNames"), allServers);
}

ServerInfo ServerInfo::load(const QString &name)
{
    QSettings settings(QStringLiteral("KDE"), QStringLiteral("UserFeedbackConsole.servers"));
    settings.beginGroup(ServerInfoData::groupName(name));

    ServerInfo info;
    info.setName(settings.value(QStringLiteral("name")).toString());
    info.setUrl(QUrl(settings.value(QStringLiteral("url")).toString()));
    info.setUserName(settings.value(QStringLiteral("userName")).toString());
    info.setPassword(settings.value(QStringLiteral("password")).toString());
    return info;
}

void ServerInfo::remove(const QString& name)
{
    QSettings settings(QStringLiteral("KDE"), QStringLiteral("UserFeedbackConsole.servers"));
    settings.remove(ServerInfoData::groupName(name));

    auto allServers = settings.value(QStringLiteral("ServerNames")).toStringList();
    allServers.removeAll(name);
    settings.setValue(QStringLiteral("ServerNames"), allServers);
}

QStringList ServerInfo::allServerInfoNames()
{
    QSettings settings(QStringLiteral("KDE"), QStringLiteral("UserFeedbackConsole.servers"));
    return settings.value(QStringLiteral("ServerNames")).toStringList();
}
