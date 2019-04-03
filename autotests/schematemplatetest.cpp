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

#include <core/aggregation.h>
#include <core/product.h>
#include <core/schemaentry.h>
#include <core/schemaentryelement.h>
#include <core/schemaentrytemplates.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace KUserFeedback::Console;

class SchemaTemplateTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schematemplates);
        QStandardPaths::setTestModeEnabled(true);
    }

    void testTemplateLoading()
    {
        QVERIFY(SchemaEntryTemplates::availableTemplates().size() > 5);

        for (const auto &t : SchemaEntryTemplates::availableTemplates()) {
            qDebug() << t.name();
            QVERIFY(!t.name().isEmpty());
            QVERIFY(!t.schema().isEmpty());
            for (const auto &entry : t.schema()) {
                QVERIFY(!entry.name().isEmpty());
                QVERIFY(!entry.elements().isEmpty());
                for (const auto &elem : entry.elements()) {
                    QVERIFY(!elem.name().isEmpty());
                }
            }
            QVERIFY(!t.aggregations().isEmpty());
            for (const auto &aggr : t.aggregations()) {
                QVERIFY(!aggr.elements().isEmpty());
                for (const auto &elem : aggr.elements()) {
                    QVERIFY(!elem.schemaEntry().name().isEmpty());
                    if (elem.type() == AggregationElement::Value)
                        QVERIFY(!elem.schemaEntryElement().name().isEmpty());
                }
            }
        }
    }
};

QTEST_MAIN(SchemaTemplateTest)

#include "schematemplatetest.moc"
