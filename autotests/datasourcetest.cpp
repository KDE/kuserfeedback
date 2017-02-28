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

#include <provider/core/applicationversionsource.h>
#include <provider/core/platforminfosource.h>
#include <provider/core/propertyratiosource.h>
#include <provider/core/qtversionsource.h>
#include <provider/core/screeninfosource.h>
#include <provider/core/startcountsource.h>
#include <provider/core/usagetimesource.h>

#include <QDebug>
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
        auto obj = src.data().toMap();
        QVERIFY(obj.contains(QLatin1String("os")));
        auto v = obj.value(QLatin1String("os"));
        QCOMPARE(v.type(), QVariant::String);
        auto s = v.toString();
        QVERIFY(!s.isEmpty());

        QVERIFY(obj.contains(QLatin1String("version")));
        v = obj.value(QLatin1String("version"));
        QCOMPARE(v.type(), QVariant::String);
        s = v.toString();
        QVERIFY(!s.isEmpty());
    }

    void testScreenInfoSource()
    {
        ScreenInfoSource src;
        auto v = src.data();
        QVERIFY(v.canConvert<QVariantList>());
        auto a = v.value<QVariantList>();
        QVERIFY(a.size() > 0);

        for (int i = 0; i < a.size(); ++i) {
            v = a.at(i);
            QVERIFY(v.canConvert<QVariantMap>());
            const auto scr = v.toMap();
            QVERIFY(scr.contains(QLatin1String("height")));
            QVERIFY(scr.contains(QLatin1String("width")));
            QVERIFY(scr.contains(QLatin1String("dpi")));
        }
    }

    void testPropertyRatioSource()
    {
        PropertyRatioSource src(this, "prop", QStringLiteral("ratioSample"));
        src.addValueMapping(42, QStringLiteral("value1"));
        src.addValueMapping(23, QStringLiteral("value2"));
        QTest::qWait(1);

        auto v = src.data();
        QVERIFY(v.canConvert<QVariantMap>());
        auto o = v.toMap();
        QCOMPARE(o.size(), 0); // nothing recorded

        QTest::qWait(1200);
        v = src.data();
        o = v.toMap();
        QCOMPARE(o.size(), 1);
        QVERIFY(o.contains(QLatin1String("value1")));
        v = o.value(QLatin1String("value1")).toMap().value(QLatin1String("property"));;
        QCOMPARE(v.type(), QVariant::Double);

        setProp(23);
        QTest::qWait(1200);
        v = src.data();
        o = v.toMap();
        QCOMPARE(o.size(), 2);
        QVERIFY(o.contains(QLatin1String("value2")));
        v = o.value(QLatin1String("value2")).toMap().value(QLatin1String("property"));
        QCOMPARE(v.type(), QVariant::Double);
    }

    void testApplicationVersionSource()
    {
        ApplicationVersionSource src;
        auto v = src.data();
        QVERIFY(v.isNull());

        QCoreApplication::setApplicationVersion(QStringLiteral("1.9.84"));
        auto m = src.data().toMap();
        QVERIFY(m.contains(QLatin1String("value")));
        QCOMPARE(m.value(QLatin1String("value")).toString(), QLatin1String("1.9.84"));
    }

    void testQtVersionSource()
    {
        QtVersionSource src;
        const auto m = src.data().toMap();
        QVERIFY(m.contains(QLatin1String("value")));
        QCOMPARE(m.value(QLatin1String("value")).toString(), QLatin1String(QT_VERSION_STR));
    }

    void testStartCountSource()
    {
        Provider p;
        auto src = new StartCountSource;
        QVERIFY(!src->description().isEmpty());
        p.addDataSource(src, Provider::DetailedUsageStatistics);
        const auto m = src->data().toMap();
        QVERIFY(m.contains(QLatin1String("value")));
        QVERIFY(m.value(QLatin1String("value")).toInt() >= 1);
    }

    void testUsageTimeSource()
    {
        Provider p;
        auto src = new UsageTimeSource;
        QVERIFY(!src->description().isEmpty());
        p.addDataSource(src, Provider::DetailedUsageStatistics);
        QTest::qWait(1200);
        const auto m = src->data().toMap();
        QVERIFY(m.contains(QLatin1String("value")));
        QVERIFY(m.value(QLatin1String("value")).toInt() >= 1);
    }
};

QTEST_MAIN(DataSourceTest)

#include "datasourcetest.moc"
