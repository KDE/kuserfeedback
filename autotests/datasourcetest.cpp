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

#include <applicationversionsource.h>
#include <compilerinfosource.h>
#include <cpuinfosource.h>
#include <localeinfosource.h>
#include <platforminfosource.h>
#include <propertyratiosource.h>
#include <qtversionsource.h>
#include <screeninfosource.h>
#include <startcountsource.h>
#include <usagetimesource.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSettings>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QStandardPaths>
#endif

using namespace KUserFeedback;

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
        QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
        QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QStandardPaths::setTestModeEnabled(true);
#endif
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
            QVERIFY(scr.contains(QLatin1String("dpi")));
#endif
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

    void testMultiPropertyRatioSource()
    {
        QSettings s;
        s.remove(QStringLiteral("MultiPropSource"));
        s.beginGroup(QStringLiteral("MultiPropSource"));

        {
            setProp(5198);

            PropertyRatioSource src1(this, "prop", QStringLiteral("ratioSample"));
            src1.addValueMapping(5198, QStringLiteral("value1"));
            src1.load(&s);

            PropertyRatioSource src2(this, "prop", QStringLiteral("ratioSample"));
            src2.addValueMapping(5198, QStringLiteral("value2"));
            src2.load(&s);

            QTest::qWait(1200);

            src1.store(&s);
            src2.store(&s);
        }

        {
            PropertyRatioSource src3(this, "prop", QStringLiteral("ratioSample"));
            src3.load(&s);
            const auto map = src3.data().toMap();
            QCOMPARE(map.size(), 2);
            QVERIFY(map.contains(QStringLiteral("value1")));
            QVERIFY(map.contains(QStringLiteral("value2")));
            src3.reset(&s);
            QVERIFY(src3.data().toMap().isEmpty());
        }

        {
            PropertyRatioSource src4(this, "prop", QStringLiteral("ratioSample"));
            src4.load(&s);
            const auto map = src4.data().toMap();
            QCOMPARE(map.size(), 0);
        }
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

    void testCpuInfoSource()
    {
        CpuInfoSource src;
        const auto m = src.data().toMap();
        QVERIFY(m.contains(QLatin1String("architecture")));
        QVERIFY(!m.value(QLatin1String("architecture")).toString().isEmpty());
        QVERIFY(m.contains(QLatin1String("count")));
        QVERIFY(m.value(QLatin1String("count")).toInt() >= 1);
    }

    void testLocaleInfoSource()
    {
        LocaleInfoSource src;
        const auto m = src.data().toMap();
        QVERIFY(m.contains(QLatin1String("language")));
        QVERIFY(!m.value(QLatin1String("language")).toString().isEmpty());
        QVERIFY(m.contains(QLatin1String("region")));
        QVERIFY(!m.value(QLatin1String("region")).toString().isEmpty());
    }

    void testCompilerInfoSource()
    {
        CompilerInfoSource src;
        const auto m = src.data().toMap();
        qDebug() << m;
        QVERIFY(m.contains(QLatin1String("type")));
        QVERIFY(!m.value(QLatin1String("type")).toString().isEmpty());
        QVERIFY(m.contains(QLatin1String("version")));
        QVERIFY(!m.value(QLatin1String("version")).toString().isEmpty());
    }
};

QTEST_MAIN(DataSourceTest)

#include "datasourcetest.moc"
