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

#include <provider/core/platforminfosource.h>
#include <provider/core/propertyratiosource.h>
#include <provider/core/screeninfosource.h>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace UserFeedback;

class DataSourceTest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int prop READ prop WRITE setProp NOTIFY propChanged)
public:
    int prop() const
    {
        return m_propValue;
    }

    void setProp(int v)
    {
        m_propValue = v;
        emit propChanged();
    }

signals:
    void propChanged();

private:
    int m_propValue = 42;

private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testPlatformInfoSource()
    {
        PlatformInfoSource src;
        QJsonObject obj;
        src.toJson(obj);
        QVERIFY(obj.contains(QLatin1String("platformOS")));
        auto v = obj.value(QLatin1String("platformOS"));
        QVERIFY(v.isString());
        auto s = v.toString();
        QVERIFY(!s.isEmpty());

        QVERIFY(obj.contains(QLatin1String("platformVersion")));
        v = obj.value(QLatin1String("platformVersion"));
        QVERIFY(v.isString());
        s = v.toString();
        QVERIFY(!s.isEmpty());
    }

    void testScreenInfoSource()
    {
        ScreenInfoSource src;
        QJsonObject obj;
        src.toJson(obj);
        QVERIFY(obj.contains(QLatin1String("screens")));
        auto v = obj.value(QLatin1String("screens"));
        QVERIFY(v.isArray());
        auto a = v.toArray();
        QVERIFY(a.size() > 0);
    }

    void testPropertyRatioSource()
    {
        PropertyRatioSource src(this, "prop", QStringLiteral("ratioSample"));
        src.addValueMapping(42, QStringLiteral("value1"));
        src.addValueMapping(23, QStringLiteral("value2"));
        QTest::qWait(1);

        QJsonObject obj;
        src.toJson(obj);
        QVERIFY(obj.contains(QLatin1String("ratioSample")));
        auto v = obj.value(QLatin1String("ratioSample"));
        QVERIFY(v.isObject());
        auto o = v.toObject();
        QCOMPARE(o.size(), 0); // nothing recorded

        QTest::qWait(1000);
        obj = {};
        src.toJson(obj);
        v = obj.value(QLatin1String("ratioSample"));
        o = v.toObject();
        QCOMPARE(o.size(), 1);
        QVERIFY(o.contains(QLatin1String("value1")));
        v = o.value(QLatin1String("value1"));
        QVERIFY(v.isDouble());

        setProp(23);
        QTest::qWait(1000);
        obj = {};
        src.toJson(obj);
        v = obj.value(QLatin1String("ratioSample"));
        o = v.toObject();
        QCOMPARE(o.size(), 2);
        QVERIFY(o.contains(QLatin1String("value2")));
        v = o.value(QLatin1String("value2"));
        QVERIFY(v.isDouble());
    }
};

QTEST_MAIN(DataSourceTest)

#include "datasourcetest.moc"

