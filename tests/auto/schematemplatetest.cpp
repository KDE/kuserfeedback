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

#include <core/aggregation.h>
#include <core/product.h>
#include <core/schemaentry.h>
#include <core/schemaentryelement.h>
#include <core/schemaentrytemplates.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace UserFeedback::Analyzer;

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
