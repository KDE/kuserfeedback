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

#include <rest/restapi.h>
#include <rest/restclient.h>

#include <core/product.h>
#include <core/survey.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QNetworkReply>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>

#include <limits>

using namespace UserFeedback::Analyzer;

class SurveyApiTest : public QObject
{
    Q_OBJECT
private:
    ServerInfo testServer() const
    {
        // TODO this needs to be read from an external file, and the test needs to be skipped if not available
        ServerInfo s;
        s.setUrl(QUrl(QStringLiteral("https://feedback.volkerkrause.eu/")));
        s.setUserName(QStringLiteral("orwell"));
        s.setPassword(QStringLiteral("1984"));
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
        QStandardPaths::setTestModeEnabled(true);
    }

    void init()
    {
        RESTClient client;
        client.connectToServer(testServer());
        Product p;
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        auto reply = RESTApi::createProduct(&client, p);
        waitForFinished(reply);
    }

    void cleanup()
    {
        RESTClient client;
        client.connectToServer(testServer());
        Product p;
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        auto reply = RESTApi::deleteProduct(&client, p);
        waitForFinished(reply);
    }

    void testSurveyCRUD()
    {
        RESTClient client;
        client.connectToServer(testServer());
        QVERIFY(client.isConnected());
        Product p;
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));

        // list surveys
        auto reply = RESTApi::listSurveys(&client, p);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        auto surveys = Survey::fromJson(reply->readAll());
        QVERIFY(surveys.isEmpty());

        // add new survey
        Survey s;
        s.setName(QStringLiteral("unitTestSurvey"));
        s.setUrl(QUrl(QStringLiteral("http://www.kde.org")));
        s.setActive(false);
        reply = RESTApi::createSurvey(&client, p, s);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);

        // verify the new product is there
        reply = RESTApi::listSurveys(&client, p);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        surveys = Survey::fromJson(reply->readAll());
        QCOMPARE(surveys.size(), 1);
        QVERIFY(surveys.at(0) != s); // id is different
        s.setId(surveys.at(0).id());
        QCOMPARE(surveys.at(0), s);

        // update a survey
        s.setActive(true);
        reply = RESTApi::updateSurvey(&client, s);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);

        // verify the update worked
        reply = RESTApi::listSurveys(&client, p);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        surveys = Survey::fromJson(reply->readAll());
        QCOMPARE(surveys.size(), 1);
        QCOMPARE(s, surveys.at(0));

        // delete a survey
        reply = RESTApi::deleteSurvey(&client, s);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);

        // verify it's gone
        reply = RESTApi::listSurveys(&client, p);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        surveys = Survey::fromJson(reply->readAll());
        QVERIFY(surveys.isEmpty());
    }

    void testInvalidSurveyOperations()
    {
        RESTClient client;
        client.connectToServer(testServer());
        QVERIFY(client.isConnected());

        Product invalidProduct;
        invalidProduct.setName(QStringLiteral("org.kde.UserFeedback.Invalid"));

        // create survey for non-existing product
        Survey s;
        s.setName(QStringLiteral("invalidTestSurvey"));
        s.setUrl(QUrl(QStringLiteral("http://www.kde.org")));
        auto reply = RESTApi::createSurvey(&client, invalidProduct, s);
        QVERIFY(waitForFinished(reply));
        QVERIFY(reply->error() != QNetworkReply::NoError);

        // update a non-existing survey
        s.setId(std::numeric_limits<int>::max());
        reply = RESTApi::updateSurvey(&client, s);
        QVERIFY(waitForFinished(reply));
        qDebug() << reply->readAll();
        QEXPECT_FAIL("", "check not implemented on the server", Continue);
        QVERIFY(reply->error() != QNetworkReply::NoError);

        // delete a non-existing survey
        reply = RESTApi::deleteSurvey(&client, s);
        QVERIFY(waitForFinished(reply));
        QEXPECT_FAIL("", "check not implemented on the server", Continue);
        QVERIFY(reply->error() != QNetworkReply::NoError);
    }
};

QTEST_MAIN(SurveyApiTest)

#include "surveyapitest.moc"

