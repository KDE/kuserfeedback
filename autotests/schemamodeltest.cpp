/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#include <model/schemamodel.h>
#include <core/schemaentrytemplates.h>

#include <QAbstractItemModelTester>
#include <QDebug>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QtTest/qtest.h>

using namespace KUserFeedback::Console;

class SchemaModelTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schematemplates);
        QStandardPaths::setTestModeEnabled(true);
    }

    void testSchemaModel()
    {
        SchemaModel model;
        QAbstractItemModelTester modelTest(&model);

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
