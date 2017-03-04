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

#include <console/rest/serverinfo.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSettings>
#include <QStandardPaths>

using namespace UserFeedback;
using namespace UserFeedback::Console;

class ServerInfoTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testServerInfoLoadStore()
    {
        {
            QSettings s;
            s.remove(QStringLiteral("ServerInfo"));
        }

        {
            const auto myInfo = ServerInfo::load(QStringLiteral("https://www.kde.org/"));
            QVERIFY(myInfo.userName().isEmpty());
            QVERIFY(myInfo.password().isEmpty());
            QVERIFY(myInfo.url().isEmpty());
        }

        {
            ServerInfo myInfo;
            myInfo.setUrl(QUrl(QStringLiteral("https://www.kde.org/")));
            myInfo.setUserName(QStringLiteral("me"));
            myInfo.setPassword(QStringLiteral("myPassword"));
            myInfo.save();
        }

        {
            const auto myInfo = ServerInfo::load(QStringLiteral("https://www.kde.org/"));
            QCOMPARE(myInfo.userName(), QLatin1String("me"));
            QCOMPARE(myInfo.password(), QLatin1String("myPassword"));
            QCOMPARE(myInfo.url(), QUrl(QStringLiteral("https://www.kde.org/")));
        }
    }
};

QTEST_MAIN(ServerInfoTest)

#include "serverinfotest.moc"
