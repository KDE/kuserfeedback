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

using namespace KUserFeedback;
using namespace KUserFeedback::Console;

class ServerInfoTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
        QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
        QStandardPaths::setTestModeEnabled(true);
    }

    void testServerInfoLoadStore()
    {
        ServerInfo::remove(QStringLiteral("UnitTestServer"));

        {
            const auto myInfo = ServerInfo::load(QStringLiteral("UnitTestServer"));
            QVERIFY(!myInfo.isValid());
            QVERIFY(myInfo.name().isEmpty());
            QVERIFY(myInfo.userName().isEmpty());
            QVERIFY(myInfo.password().isEmpty());
            QVERIFY(myInfo.url().isEmpty());
        }
        QVERIFY(!ServerInfo::allServerInfoNames().contains(QStringLiteral("UnitTestServer")));

        {
            ServerInfo myInfo;
            myInfo.setName(QStringLiteral("UnitTestServer"));
            myInfo.setUrl(QUrl(QStringLiteral("https://www.kde.org/")));
            myInfo.setUserName(QStringLiteral("me"));
            myInfo.setPassword(QStringLiteral("myPassword"));
            QVERIFY(myInfo.isValid());
            myInfo.save();
        }
        QVERIFY(ServerInfo::allServerInfoNames().contains(QStringLiteral("UnitTestServer")));

        {
            const auto myInfo = ServerInfo::load(QStringLiteral("UnitTestServer"));
            QVERIFY(myInfo.isValid());
            QCOMPARE(myInfo.name(), QLatin1String("UnitTestServer"));
            QCOMPARE(myInfo.userName(), QLatin1String("me"));
            QCOMPARE(myInfo.password(), QLatin1String("myPassword"));
            QCOMPARE(myInfo.url(), QUrl(QStringLiteral("https://www.kde.org/")));
        }

        ServerInfo::remove(QStringLiteral("UnitTestServer"));
    }
};

QTEST_MAIN(ServerInfoTest)

#include "serverinfotest.moc"
