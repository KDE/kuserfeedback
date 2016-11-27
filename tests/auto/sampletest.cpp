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
            "timestampe": "2016-11-27 16:09:06",
            "platform": { "os": "linux", "version": "suse" },
            "applicationVersion": { "value": "1.9.84" },
            "screens": [ { "width": 1920, "height": 1200 }, { "width": 1920, "height": 1200 } ]
        }])", p);

        QCOMPARE(samples.size(), 1);
        auto s = samples.at(0);
    }
};

QTEST_MAIN(SampleTest)

#include "sampletest.moc"
