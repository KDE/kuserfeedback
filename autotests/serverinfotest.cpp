/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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
