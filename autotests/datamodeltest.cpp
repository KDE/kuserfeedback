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

#include <analyzer/core/sample.h>
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
private:
    int findColumn(QAbstractItemModel *model, const QString &name)
    {
        for (int i = 0; i < model->columnCount(); ++i) {
            if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() == name)
                return i;
        }
        return -1;
    }

private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schematemplates);
        QStandardPaths::setTestModeEnabled(true);
    }

    void testEmptyDataModel()
    {
        DataModel model;
        ModelTest modelTest(&model);

        model.setProduct({});
        QCOMPARE(model.rowCount(), 0);
        QCOMPARE(model.columnCount(), 1);

        Product p;
        for (const auto &tpl : SchemaEntryTemplates::availableTemplates())
            p.addTemplate(tpl);
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTest"));
        model.setProduct(p);
        QVERIFY(model.columnCount() > 8);
    }

    void testDataModelContent()
    {
        DataModel model;
        ModelTest modelTest(&model);
        Product p;
        for (const auto &tpl : SchemaEntryTemplates::availableTemplates())
            p.addTemplate(tpl);
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTest"));
        model.setProduct(p);

        auto samples = Sample::fromJson(R"([{
            "id": 42,
            "timestamp": "2016-11-27 16:09:06",
            "platform": { "os": "linux", "version": "suse" },
            "applicationVersion": { "value": "1.9.84" },
            "screens": [ { "width": 1920, "height": 1200 }, { "width": 640, "height": 480 } ],
            "newPropertyRatio": { "value1": { "property": 0.55 }, "value2": { "property": 0.45 } }
        }, {
            "id": 43,
            "timestamp": "2016-12-09 12:00:00",
            "platform": { "os": "linux", "version": "suse" },
            "applicationVersion": { "value": "1.9.84" },
            "screens": [ { "width": 1920, "height": 1200 } ],
            "newPropertyRatio": { "value1": { "property": 0.1 }, "value2": { "property": 0.9 } }
        }])", p);
        QCOMPARE(samples.size(), 2);
        model.setSamples(samples);
        QCOMPARE(model.rowCount(), 2);

        auto colIdx = findColumn(&model, QLatin1String("platform.os"));
        QCOMPARE(model.index(0, colIdx).data(Qt::DisplayRole).toString(), QLatin1String("linux"));
        colIdx = findColumn(&model, QLatin1String("platform.version"));
        QCOMPARE(model.index(0, colIdx).data(Qt::DisplayRole).toString(), QLatin1String("suse"));
        colIdx = findColumn(&model, QLatin1String("applicationVersion.value"));
        QCOMPARE(model.index(0, colIdx).data(Qt::DisplayRole).toString(), QLatin1String("1.9.84"));

        colIdx = findColumn(&model, QLatin1String("screens"));
        QCOMPARE(model.index(0, colIdx).data(Qt::DisplayRole).toString(), QLatin1String("[{height: 1200, width: 1920}, {height: 480, width: 640}]"));

        colIdx = findColumn(&model, QLatin1String("newPropertyRatio"));
        QCOMPARE(model.index(0, colIdx).data(Qt::DisplayRole).toString(), QLatin1String("value1: {property: 0.55}, value2: {property: 0.45}"));

        const auto sample = model.index(1, 1).data(DataModel::SampleRole).value<Sample>();
        QCOMPARE(sample.timestamp().date(), QDate(2016, 12, 9));
    }
};

QTEST_MAIN(DataModelTest)

#include "datamodeltest.moc"
