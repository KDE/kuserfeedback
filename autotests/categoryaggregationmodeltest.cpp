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

#include <console/model/categoryaggregationmodel.h>
#include <console/model/datamodel.h>
#include <console/model/timeaggregationmodel.h>
#include <console/core/sample.h>
#include <console/core/schemaentrytemplates.h>

#include <3rdparty/qt/modeltest.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QStandardPaths>

using namespace UserFeedback::Console;

class CategoryAggregationModelTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(schematemplates);
        QStandardPaths::setTestModeEnabled(true);
    }

    void testEmptyModel()
    {
        CategoryAggregationModel model;
        ModelTest modelTest(&model);
        model.setAggregationValue(QString());
        model.setAggregationValue(QLatin1String("applicationVersion.value"));

        TimeAggregationModel timeModel;
        model.setSourceModel(&timeModel);

        DataModel srcModel;
        timeModel.setSourceModel(&srcModel);
        srcModel.setProduct({});
        QCOMPARE(model.rowCount(), 0);
        QCOMPARE(model.columnCount(), 1);

        Product p;
        for (const auto &tpl : SchemaEntryTemplates::availableTemplates())
            p.addTemplate(tpl);
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTest"));
        srcModel.setProduct(p);
        QCOMPARE(model.columnCount(), 1);
        QCOMPARE(model.rowCount(), 0);
    }

    void testModelContent()
    {
        CategoryAggregationModel model;
        ModelTest modelTest(&model);
        model.setAggregationValue(QLatin1String("applicationVersion.value"));

        TimeAggregationModel timeModel;
        model.setSourceModel(&timeModel);

        DataModel srcModel;
        timeModel.setSourceModel(&srcModel);
        timeModel.setAggregationMode(TimeAggregationModel::AggregateDay);
        Product p;
        for (const auto &tpl : SchemaEntryTemplates::availableTemplates())
            p.addTemplate(tpl);
        p.setName(QStringLiteral("org.kde.UserFeedback.UnitTest"));
        srcModel.setProduct(p);

        auto samples = Sample::fromJson(R"([
            { "timestamp": "2016-11-27 12:00:00", "applicationVersion": { "value": "1.0" } },
            { "timestamp": "2016-11-27 12:00:00", "applicationVersion": { "value": "1.9.84" } },
            { "timestamp": "2016-11-27 12:00:00", "applicationVersion": { "value": "1.9.84" } },
            { "timestamp": "2016-11-28 12:00:00", "applicationVersion": { "value": "1.9.84" } },
            { "timestamp": "2016-11-28 12:00:00", "applicationVersion": { "value": "1.9.84" } },
            { "timestamp": "2016-11-28 12:00:00" }
        ])", p);
        QCOMPARE(samples.size(), 6);
        srcModel.setSamples(samples);

        QCOMPARE(model.columnCount(), 4);
        QCOMPARE(model.headerData(1, Qt::Horizontal, Qt::DisplayRole).toString(), QLatin1String("[empty]"));
        QCOMPARE(model.headerData(2, Qt::Horizontal, Qt::DisplayRole).toString(), QLatin1String("1.0"));
        QCOMPARE(model.headerData(3, Qt::Horizontal, Qt::DisplayRole).toString(), QLatin1String("1.9.84"));

        QCOMPARE(model.rowCount(), 2);
        QCOMPARE(model.index(0, 0).data(TimeAggregationModel::TimeDisplayRole).toString(), QLatin1String("2016-11-27"));
        QCOMPARE(model.index(0, 1).data(Qt::DisplayRole).toInt(), 0);
        QCOMPARE(model.index(0, 2).data(Qt::DisplayRole).toInt(), 1);
        QCOMPARE(model.index(0, 3).data(Qt::DisplayRole).toInt(), 2);
        QCOMPARE(model.index(0, 3).data(TimeAggregationModel::AccumulatedDisplayRole).toInt(), 3);

        QCOMPARE(model.index(1, 0).data(TimeAggregationModel::TimeDisplayRole).toString(), QLatin1String("2016-11-28"));
        QCOMPARE(model.index(1, 1).data(Qt::DisplayRole).toInt(), 1);
        QCOMPARE(model.index(1, 2).data(Qt::DisplayRole).toInt(), 0);
        QCOMPARE(model.index(0, 3).data(Qt::DisplayRole).toInt(), 2);
        QCOMPARE(model.index(0, 3).data(TimeAggregationModel::AccumulatedDisplayRole).toInt(), 3);

        QCOMPARE(model.index(0, 0).data(TimeAggregationModel::MaximumValueRole).toInt(), 3);
    }
};

QTEST_MAIN(CategoryAggregationModelTest)

#include "categoryaggregationmodeltest.moc"
