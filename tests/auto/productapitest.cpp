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

#include <QDebug>
#include <QtTest/qtest.h>
#include <QNetworkReply>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>

using namespace UserFeedback::Analyzer;

class ProductApiTest : public QObject
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

    Product findProduct(const QVector<Product> &products, const QString &name)
    {
        const auto it = std::find_if(products.cbegin(), products.cend(), [name](const Product &p) {
            return p.name() == name;
        });
        if (it != products.cend())
            return *it;
        return {};
    }

private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testInvalidList()
    {
        RESTClient client;
        auto serverInfo = testServer();
        serverInfo.setPassword(QStringLiteral("wrong password"));
        client.connectToServer(serverInfo);
        QVERIFY(client.isConnected());

        auto reply = RESTApi::getProducts(&client);
        QVERIFY(waitForFinished(reply));
        QVERIFY(reply->error() != QNetworkReply::NoError);
    }

    void testProductCRUD()
    {
        RESTClient client;
        client.connectToServer(testServer());
        QVERIFY(client.isConnected());
        Product newProduct;
        newProduct.setName(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));

        // clean up from previous failed runs, if needed
        auto reply = RESTApi::deleteProduct(&client, newProduct);
        waitForFinished(reply);

        // list existing products
        reply = RESTApi::getProducts(&client);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        const auto products = Product::fromJson(reply->readAll());
        QVERIFY(!findProduct(products, QLatin1String("org.kde.UserFeedback.UnitTestProduct")).isValid());

        // add new product
        SchemaEntry entry;
        entry.setName(QStringLiteral("entry1"));
        entry.setType(SchemaEntry::StringType);
        entry.setAggregationType(SchemaEntry::Category);
        newProduct.setSchema({entry});
        QVERIFY(newProduct.isValid());
        reply = RESTApi::createProduct(&client, newProduct);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);

        // verify the new product is there
        reply = RESTApi::getProducts(&client);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        auto newProducts = Product::fromJson(reply->readAll());
        QCOMPARE(newProducts.size(), products.size() + 1);
        const auto createdProduct = findProduct(newProducts, QLatin1String("org.kde.UserFeedback.UnitTestProduct"));
        QVERIFY(createdProduct.isValid());
        QCOMPARE(createdProduct.schema(), newProduct.schema());

        // update a product
        reply = RESTApi::updateProduct(&client, newProduct);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);

        // try to add an already existing product
        reply = RESTApi::createProduct(&client, newProduct);
        QVERIFY(waitForFinished(reply));
        QVERIFY(reply->error() != QNetworkReply::NoError);

        // verify it really wasn't added
        reply = RESTApi::getProducts(&client);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        newProducts = Product::fromJson(reply->readAll());
        QCOMPARE(newProducts.size(), products.size() + 1);

        // delete a product
        reply = RESTApi::deleteProduct(&client, newProduct);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);

        // verify it's gone, and nothing else was harmed
        reply = RESTApi::getProducts(&client);
        QVERIFY(waitForFinished(reply));
        QCOMPARE(reply->error(), QNetworkReply::NoError);
        newProducts = Product::fromJson(reply->readAll());
        QCOMPARE(newProducts.size(), products.size());
        QVERIFY(!findProduct(newProducts, QLatin1String("org.kde.UserFeedback.UnitTestProduct")).isValid());
    }

    void testInvalidProductOperations()
    {
        RESTClient client;
        client.connectToServer(testServer());
        QVERIFY(client.isConnected());

        Product invalidProduct;
        invalidProduct.setName(QStringLiteral("org.kde.UserFeedback.Invalid"));
        QVERIFY(invalidProduct.isValid());

        // update a non-existing product
        auto reply = RESTApi::updateProduct(&client, invalidProduct);
        QVERIFY(waitForFinished(reply));
        qDebug() << reply->readAll();
        QVERIFY(reply->error() != QNetworkReply::NoError);

        // delete a non-existing product
        reply = RESTApi::deleteProduct(&client, invalidProduct);
        QVERIFY(waitForFinished(reply));
        QVERIFY(reply->error() != QNetworkReply::NoError);
    }
};

QTEST_MAIN(ProductApiTest)

#include "productapitest.moc"
