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

#include "servercontroller.h"

#include <rest/restapi.h>
#include <rest/restclient.h>

#include <provider/core/provider.h>
#include <provider/core/platforminfosource.h>
#include <provider/core/screeninfosource.h>

#include <core/product.h>
#include <core/schemaentryelement.h>
#include <core/schemaentrytemplates.h>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtTest/qtest.h>
#include <QNetworkReply>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>

using namespace UserFeedback;
using namespace UserFeedback::Console;

class ProviderTest : public QObject
{
    Q_OBJECT
private:
    ServerController m_server;

    ServerInfo testServer() const
    {
        ServerInfo s;
        s.setUrl(m_server.url());
        return s;
    }

    bool waitForFinished(QNetworkReply *reply)
    {
        Q_ASSERT(reply);
        QSignalSpy spy(reply, &QNetworkReply::finished);
        Q_ASSERT(spy.isValid());
        return spy.wait();
    }

private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schematemplates);
        QStandardPaths::setTestModeEnabled(true);
        QVERIFY(m_server.start());
    }

    void testProvider()
    {
        // delete previous leftovers
        RESTClient client;
        client.connectToServer(testServer());
        QVERIFY(client.isConnected());
        Product p;
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        auto reply = RESTApi::deleteProduct(&client, p);
        waitForFinished(reply);

        // create test product
        for (const auto &tpl : SchemaEntryTemplates::availableTemplates())
            p.addTemplate(tpl);
        QVERIFY(p.isValid());
        reply = RESTApi::createProduct(&client, p);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);

        // submit data
        Provider provider;
        provider.setStatisticsCollectionMode(Provider::AllStatistics); // TODO test with no stats, should not create a sample record
        provider.setProductIdentifier(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        provider.setFeedbackServer(m_server.url());
        provider.addDataSource(new ScreenInfoSource, Provider::AllStatistics);
        provider.addDataSource(new PlatformInfoSource, Provider::AllStatistics);
        provider.submit();
        QTest::qWait(100); // HACK submit is async

        // retrieve data
        reply = RESTApi::listSamples(&client, p);
        QVERIFY(waitForFinished(reply));
        QVERIFY(reply->header(QNetworkRequest::ContentTypeHeader).toString().startsWith(QLatin1String("text/json")));
        auto doc = QJsonDocument::fromJson(reply->readAll());
        QVERIFY(doc.isArray());
        auto a = doc.array();
        QCOMPARE(a.size(), 1);

        auto obj = a.at(0).toObject();
        QVERIFY(!obj.isEmpty());
        QVERIFY(obj.contains(QLatin1String("id")));
        QVERIFY(obj.contains(QLatin1String("timestamp")));

        QVERIFY(obj.contains(QLatin1String("platform")));
        auto sub = obj.value(QLatin1String("platform")).toObject();
        QVERIFY(sub.contains(QLatin1String("os")));
        QVERIFY(sub.contains(QLatin1String("version")));

        QVERIFY(obj.contains(QLatin1String("screens")));
        a = obj.value(QLatin1String("screens")).toArray();
        QVERIFY(a.size() > 0);
        sub = a.at(0).toObject();
        QVERIFY(sub.contains(QLatin1String("height")));
        QVERIFY(sub.contains(QLatin1String("width")));
    }
};

QTEST_MAIN(ProviderTest)

#include "providertest.moc"
