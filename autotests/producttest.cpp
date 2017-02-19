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

#include <console/core/aggregation.h>
#include <console/core/product.h>

#include <3rdparty/qt/modeltest.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace UserFeedback::Console;

class ProductTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testFromJson()
    {
        const auto ps = Product::fromJson(R"({
            "name": "org.kde.TestProduct",
            "schema": [{
                "name": "entry1",
                "type": "scalar",
                "elements": [{ "name": "elem11", "type": "string" }]
            }, {
                "name": "entry2",
                "type": "list",
                "elements": [{ "name": "elem21", "type": "number" }]
            }],
            "aggregation": [{
                "type": "ratio_set",
                "elements": [{ "type": "value", "schemaEntry": "entry1", "schemaEntryElement": "elem11" }]
            }, {
                "type": "numeric",
                "elements": [{ "type": "size", "schemaEntry": "entry2" }]
            }]
        })");

        QCOMPARE(ps.size(), 1);

        const auto p = ps.at(0);
        QCOMPARE(p.name(), QLatin1String("org.kde.TestProduct"));

        QCOMPARE(p.schema().size(), 2);

        const auto aggrs = p.aggregations();
        QCOMPARE(aggrs.size(), 2);
        {
            const auto a1 = aggrs.at(0);
            QCOMPARE(a1.type(), Aggregation::RatioSet);
            const auto a1elems = a1.elements();
            QCOMPARE(a1elems.size(), 1);
            QCOMPARE(a1elems.at(0).type(), AggregationElement::Value);
            QCOMPARE(a1elems.at(0).schemaEntry().name(), QLatin1String("entry1"));
            QCOMPARE(a1elems.at(0).schemaEntry().dataType(), SchemaEntry::Scalar);
            QCOMPARE(a1elems.at(0).schemaEntryElement().name(), QLatin1String("elem11"));
        }

        {
            const auto a2 = aggrs.at(1);
            QCOMPARE(a2.type(), Aggregation::Numeric);
            const auto a2elems = a2.elements();
            QCOMPARE(a2elems.size(), 1);
            QCOMPARE(a2elems.at(0).type(), AggregationElement::Size);
            QCOMPARE(a2elems.at(0).schemaEntry().name(), QLatin1String("entry2"));
            QCOMPARE(a2elems.at(0).schemaEntry().dataType(), SchemaEntry::List);
            QVERIFY(a2elems.at(0).schemaEntryElement().name().isEmpty());
        }
    }

};

QTEST_MAIN(ProductTest)

#include "producttest.moc"
