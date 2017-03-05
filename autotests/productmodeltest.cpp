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
#include <model/productmodel.h>

#include <3rdparty/qt/modeltest.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>

using namespace UserFeedback::Console;

class ProductModelTest : public QObject
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
        QStandardPaths::setTestModeEnabled(true);
        QVERIFY(m_server.start());
    }

    void testProductModel()
    {
        RESTClient client;
        client.setServerInfo(testServer());
        client.setConnected(true);
        QVERIFY(client.isConnected());

        // make sure we have at least one product, but not the one we want to create for testing
        Product p;
        p.setName(QStringLiteral("org.kde.SomeOtherTestProduct"));
        auto reply = RESTApi::createProduct(&client, p);
        waitForFinished(reply);
        p.setName(QStringLiteral("org.kde.NewUnitTestProduct"));
        reply = RESTApi::deleteProduct(&client, p);
        waitForFinished(reply);

        ProductModel model;
        ModelTest modelTest(&model);
        QSignalSpy resetSpy(&model, &ProductModel::modelReset);
        QSignalSpy insertSpy(&model, &ProductModel::rowsInserted);
        QSignalSpy removeSpy(&model, &ProductModel::rowsRemoved);

        model.setRESTClient(&client);
        QVERIFY(insertSpy.wait());
        const auto baseCount = model.rowCount();

        insertSpy.clear();
        reply = RESTApi::createProduct(&client, p);
        QVERIFY(waitForFinished(reply));
        resetSpy.clear();
        model.reload();
        QVERIFY(insertSpy.wait());
        QCOMPARE(model.rowCount(), baseCount + 1);

        reply = RESTApi::deleteProduct(&client, p);
        QVERIFY(waitForFinished(reply));
        resetSpy.clear();
        model.reload();
        QVERIFY(removeSpy.wait());
        QCOMPARE(model.rowCount(), baseCount);

        removeSpy.clear();
        model.clear();
        QCOMPARE(removeSpy.size(), 1);
        QVERIFY(resetSpy.isEmpty());
    }
};

QTEST_MAIN(ProductModelTest)

#include "productmodeltest.moc"
