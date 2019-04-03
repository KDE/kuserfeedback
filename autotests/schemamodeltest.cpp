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

#include <model/schemamodel.h>
#include <core/schemaentrytemplates.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
#include <QAbstractItemModelTester>
#endif

using namespace KUserFeedback::Console;

class SchemaModelTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schematemplates);
        QStandardPaths::setTestModeEnabled(true);
    }

    void testSchemaModel()
    {
        SchemaModel model;
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        QAbstractItemModelTester modelTest(&model);
#endif

        model.setProduct(Product());
        QCOMPARE(model.rowCount(), 0);

        Product p;
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTest"));
        model.setProduct(p);
        QCOMPARE(model.rowCount(), 0);

        QVector<SchemaEntry> schema;
        SchemaEntry entry;
        entry.setName(QStringLiteral("entry1"));
        schema.push_back(entry);
        p.setSchema(schema);
        model.setProduct(p);
        QCOMPARE(model.rowCount(), schema.size());

        for (const auto &tpl : SchemaEntryTemplates::availableTemplates())
            p.addTemplate(tpl);
        model.setProduct(p);
        QVERIFY(model.rowCount() > 0);
    }
};

QTEST_MAIN(SchemaModelTest)

#include "schemamodeltest.moc"
