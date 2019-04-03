/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include <selectionratiosource.h>

#include <QDebug>
#include <QItemSelectionModel>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardPaths>

using namespace KUserFeedback;

class SelectionRatioSourceTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
        QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
        QStandardPaths::setTestModeEnabled(true);
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
