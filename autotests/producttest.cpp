/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <console/core/aggregation.h>
#include <console/core/product.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace KUserFeedback::Console;

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
