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

#include <core/product.h>
#include <core/sample.h>
#include <core/schemaentrytemplates.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace UserFeedback::Analyzer;

class SampleTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schemaentrytemplates);
        QStandardPaths::setTestModeEnabled(true);
    }

    void testFromJson()
    {
        Product p;
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        p.setSchema(SchemaEntryTemplates::availableTemplates());
        QVERIFY(p.isValid());

        auto samples = Sample::fromJson("[]", p);
        QVERIFY(samples.isEmpty());

        samples = Sample::fromJson(R"([{
            "id": 42,
            "timestamp": "2016-11-27 16:09:06",
            "platform": { "os": "linux", "version": "suse" },
            "applicationVersion": { "value": "1.9.84" },
            "screens": [ { "width": 1920, "height": 1200 }, { "width": 640, "height": 480 } ],
            "newPropertyRatio": { "value1": { "property": 0.55 }, "value2": { "property": 0.45 } }
        }])", p);

        QCOMPARE(samples.size(), 1);
        auto s = samples.at(0);
        QCOMPARE(s.timestamp(), QDateTime(QDate(2016, 11, 27), QTime(16, 9, 6)));
        QCOMPARE(s.value(QLatin1String("platform.os")).toString(), QLatin1String("linux"));
        QCOMPARE(s.value(QLatin1String("platform.version")).toString(), QLatin1String("suse"));
        QCOMPARE(s.value(QLatin1String("applicationVersion.value")).toString(), QLatin1String("1.9.84"));
        auto screens = s.value(QLatin1String("screens")).toList();
        QCOMPARE(screens.size(), 2);
        QCOMPARE(screens.at(0).toMap().value(QLatin1String("width")).toInt(), 1920);
        QCOMPARE(screens.at(1).toMap().value(QLatin1String("height")).toInt(), 480);
        auto ratio = s.value(QLatin1String("newPropertyRatio")).toMap();
        QCOMPARE(ratio.size(), 2);
        QCOMPARE(ratio.value(QLatin1String("value1")).toMap().value(QLatin1String("property")).toDouble(), 0.55);
        QCOMPARE(ratio.value(QLatin1String("value2")).toMap().value(QLatin1String("property")).toDouble(), 0.45);
    }
};

QTEST_MAIN(SampleTest)

#include "sampletest.moc"
