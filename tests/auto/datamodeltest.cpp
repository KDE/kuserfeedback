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

#include <analyzer/core/schemaentrytemplates.h>
#include <analyzer/model/datamodel.h>

#include <3rdparty/qt/modeltest.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace UserFeedback::Analyzer;

class DataModelTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schemaentrytemplates);
        QStandardPaths::setTestModeEnabled(true);
    }

    void testDataModel()
    {
        DataModel model;
        ModelTest modelTest(&model);

        model.setProduct({});
        QCOMPARE(model.rowCount(), 0);
        QCOMPARE(model.columnCount(), 1);

        Product p;
        p.setSchema(SchemaEntryTemplates::availableTemplates());
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTest"));
        model.setProduct(p);
        QVERIFY(model.columnCount() > 8);
    }
};

QTEST_MAIN(DataModelTest)

#include "datamodeltest.moc"
