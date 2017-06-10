/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include <selectionratiosource.h>

#include <QDebug>
#include <QItemSelectionModel>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSettings>
#include <QStandardItemModel>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QStandardPaths>
#endif

using namespace KUserFeedback;

class SelectionRatioSourceTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
        QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QStandardPaths::setTestModeEnabled(true);
#endif
    }

    void testPropertyRatioSource()
    {
        QStandardItemModel model;
        model.appendRow(new QStandardItem(QStringLiteral("row1")));
        model.appendRow(new QStandardItem(QStringLiteral("row2")));
        model.appendRow(new QStandardItem(QStringLiteral("row3")));

        QItemSelectionModel sel(&model);
        sel.select(model.index(1, 0), QItemSelectionModel::ClearAndSelect);

        SelectionRatioSource src(&sel, QStringLiteral("selection"));
        QTest::qWait(1);

        auto v = src.data();
        QVERIFY(v.canConvert<QVariantMap>());
        auto o = v.toMap();
        QCOMPARE(o.size(), 0); // nothing recorded

        QTest::qWait(1200);
        v = src.data();
        o = v.toMap();
        QCOMPARE(o.size(), 1);
        QVERIFY(o.contains(QLatin1String("row2")));
        v = o.value(QLatin1String("row2")).toMap().value(QLatin1String("property"));
        QCOMPARE(v.type(), QVariant::Double);

        sel.select(model.index(2, 0), QItemSelectionModel::ClearAndSelect);
        QTest::qWait(1200);
        v = src.data();
        o = v.toMap();
        QCOMPARE(o.size(), 2);
        QVERIFY(o.contains(QLatin1String("row2")));
        v = o.value(QLatin1String("row2")).toMap().value(QLatin1String("property"));
        QCOMPARE(v.type(), QVariant::Double);
    }
};

QTEST_MAIN(SelectionRatioSourceTest)

#include "selectionratiosourcetest.moc"
