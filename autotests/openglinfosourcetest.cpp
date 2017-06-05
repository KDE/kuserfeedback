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

#include <openglinfosource.h>
#include <src/provider/core/openglinfosource_p.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QStandardPaths>
#endif

using namespace UserFeedback;

class OpenGLInfoSourceTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QStandardPaths::setTestModeEnabled(true);
#endif
    }

    void testOpenGLInfoSource()
    {
        OpenGLInfoSource src;
        const auto m = src.data().toMap();
        QVERIFY(m.contains(QLatin1String("type")));
        const auto type = m.value(QLatin1String("type")).toString();
        QVERIFY(!type.isEmpty());
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QVERIFY(type == QLatin1String("GL") || type == QLatin1String("GLES"));

        QVERIFY(m.contains(QLatin1String("vendor")));
        QVERIFY(!m.value(QLatin1String("vendor")).toString().isEmpty());
        QVERIFY(m.contains(QLatin1String("renderer")));
        QVERIFY(!m.value(QLatin1String("renderer")).toString().isEmpty());
        QVERIFY(m.contains(QLatin1String("version")));
        QVERIFY(!m.value(QLatin1String("version")).toString().isEmpty());
        QVERIFY(m.contains(QLatin1String("vendorVersion")));
        QVERIFY(!m.value(QLatin1String("vendorVersion")).toString().isEmpty());
        QVERIFY(m.contains(QLatin1String("glslVersion")));
        QVERIFY(!m.value(QLatin1String("glslVersion")).toString().isEmpty());
#endif
    }

    void testParseGLVersion_data()
    {
        QTest::addColumn<QString>("glVersion");
        QTest::addColumn<QString>("vendorVersion");

        QTest::newRow("empty") << QString() << QString();
        QTest::newRow("default") << QStringLiteral("3.0") << QString();
        QTest::newRow("mesa") << QStringLiteral("3.0 Mesa 17.1.1") << QStringLiteral("Mesa 17.1.1");
        QTest::newRow("nvidia") << QStringLiteral("4.5 NVIDIA 375.26") << QStringLiteral("NVIDIA 375.26");
    }

    void testParseGLVersion()
    {
        QFETCH(QString, glVersion);
        QFETCH(QString, vendorVersion);

        QVariantMap m;
        OpenGLInfoSourcePrivate::parseGLVersion(glVersion.toUtf8(), m);
        QCOMPARE(m.value(QLatin1String("vendorVersion")).toString(), vendorVersion);
   }

    void testParseGLESVersion_data()
    {
        QTest::addColumn<QString>("glVersion");
        QTest::addColumn<QString>("version");
        QTest::addColumn<QString>("vendorVersion");

        QTest::newRow("empty") << QString() << QStringLiteral("unknown") << QString();
        QTest::newRow("invalid") << QStringLiteral("OpenGL 2.0") << QStringLiteral("unknown") << QStringLiteral("OpenGL 2.0");
        QTest::newRow("default") << QStringLiteral("OpenGL ES 2.0") << QStringLiteral("2.0") << QString();
        QTest::newRow("angle") << QStringLiteral("OpenGL ES 2.0 (ANGLE 2.1.0.8613f4946861)") << QStringLiteral("2.0") << QStringLiteral("ANGLE 2.1.0.8613f4946861");
    }

    void testParseGLESVersion()
    {
        QFETCH(QString, glVersion);
        QFETCH(QString, version);
        QFETCH(QString, vendorVersion);

        QVariantMap m;
        OpenGLInfoSourcePrivate::parseGLESVersion(glVersion.toUtf8(), m);
        QCOMPARE(m.value(QLatin1String("version")).toString(), version);
        QCOMPARE(m.value(QLatin1String("vendorVersion")).toString(), vendorVersion);
   }
};

QTEST_MAIN(OpenGLInfoSourceTest)

#include "openglinfosourcetest.moc"
