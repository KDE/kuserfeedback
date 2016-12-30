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

#include <model/schemamodel.h>
#include <core/schemaentrytemplates.h>

#include <3rdparty/qt/modeltest.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSignalSpy>
#include <QStandardPaths>

using namespace UserFeedback::Analyzer;

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
        ModelTest modelTest(&model);

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
