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

#include <rest/restclient.h>
#include <model/productmodel.h>

#include <3rdparty/qt/modeltest.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>

using namespace UserFeedback::Analyzer;

class ProductModelTest : public QObject
{
    Q_OBJECT
private:
    ServerController m_server;

    ServerInfo testServer() const
    {
        ServerInfo s;
        s.setUrl(m_server.url());
        s.setUserName(QStringLiteral("orwell"));
        s.setPassword(QStringLiteral("1984"));
        return s;
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
        client.connectToServer(testServer());
        QVERIFY(client.isConnected());

        ProductModel model;
        ModelTest modelTest(&model);

        model.setRESTClient(&client);
        QSignalSpy spy(&model, &ProductModel::modelReset);
        QVERIFY(spy.wait());
    }
};

QTEST_MAIN(ProductModelTest)

#include "productmodeltest.moc"
