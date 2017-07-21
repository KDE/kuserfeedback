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

using namespace KUserFeedback;

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
        QTest::addColumn<QString>("version");

        QTest::newRow("empty") << QString() << QString() << QString();
        QTest::newRow("default") << QStringLiteral("3.0") << QString() << QStringLiteral("3.0");
        QTest::newRow("mesa") << QStringLiteral("3.0 Mesa 17.1.1") << QStringLiteral("Mesa 17.1.1") << QStringLiteral("3.0");
        QTest::newRow("nvidia") << QStringLiteral("4.5 NVIDIA 375.26") << QStringLiteral("NVIDIA 375.26") << QStringLiteral("4.5");;
        QTest::newRow("intel osx") << QStringLiteral("2.1 INTEL-10.25.13") << QStringLiteral("INTEL-10.25.13") << QStringLiteral("2.1");;
        QTest::newRow("intel win") << QStringLiteral("4.3 - Build 20.19.15.4568") << QStringLiteral("Build 20.19.15.4568") << QStringLiteral("4.3");
    }

    void testParseGLVersion()
    {
        QFETCH(QString, glVersion);
        QFETCH(QString, vendorVersion);
        QFETCH(QString, version);

        QVariantMap m;
        OpenGLInfoSourcePrivate::parseGLVersion(glVersion.toUtf8(), m);
        QCOMPARE(m.value(QLatin1String("vendorVersion")).toString(), vendorVersion);
        QCOMPARE(m.value(QLatin1String("version")).toString(), version);

        // don't overwrite version if we could correctly determine this by other means already
        m.clear();
        m.insert(QStringLiteral("version"), QStringLiteral("12345.6789"));
        OpenGLInfoSourcePrivate::parseGLVersion(glVersion.toUtf8(), m);
        QCOMPARE(m.value(QLatin1String("vendorVersion")).toString(), vendorVersion);
        QCOMPARE(m.value(QLatin1String("version")).toString(), QLatin1String("12345.6789"));
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

    void testParseGLSLVersion_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("glslVersion");

        QTest::newRow("empty") << QString() << QString();
        QTest::newRow("default") << QStringLiteral("1.30") << QStringLiteral("1.30");
        QTest::newRow("nvidia") << QStringLiteral("4.50 NVIDIA") << QStringLiteral("4.50");
    }

    void testParseGLSLVersion()
    {
        QFETCH(QString, input);
        QFETCH(QString, glslVersion);

        QVariantMap m;
        OpenGLInfoSourcePrivate::parseGLSLVersion(input.toUtf8(), m);
        QCOMPARE(m.value(QLatin1String("glslVersion")).toString(), glslVersion);
   }

    void testParseESGLSLVersion_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("glslVersion");

        QTest::newRow("empty") << QString() << QString();
        QTest::newRow("invalid") << QStringLiteral("GLSL 1.00") << QStringLiteral("GLSL 1.00");
        QTest::newRow("default") << QStringLiteral("OpenGL ES GLSL ES 1.00") << QStringLiteral("1.00");
        QTest::newRow("angle") << QStringLiteral("OpenGL ES GLSL ES 1.00 (ANGLE 2.1.0.8613f4946861)") << QStringLiteral("1.00");
    }

    void testParseESGLSLVersion()
    {
        QFETCH(QString, input);
        QFETCH(QString, glslVersion);

        QVariantMap m;
        OpenGLInfoSourcePrivate::parseESGLSLVersion(input.toUtf8(), m);
        QCOMPARE(m.value(QLatin1String("glslVersion")).toString(), glslVersion);
   }

   void testNormalizeVendor_data()
   {
       QTest::addColumn<QString>("input");
       QTest::addColumn<QString>("vendor");

       QTest::newRow("empty") << QString() << QString();
       QTest::newRow("intel window") << QStringLiteral("Intel") << QStringLiteral("Intel");
       QTest::newRow("intel linux") << QStringLiteral("Intel Open Source Technology Center") << QStringLiteral("Intel");
       QTest::newRow("intel macOS") << QStringLiteral("Intel Inc.") << QStringLiteral("Intel");
       QTest::newRow("nvidia") << QStringLiteral("NVIDIA Corporation") << QStringLiteral("NVIDIA Corporation");
   }

   void testNormalizeVendor()
   {
       QFETCH(QString, input);
       QFETCH(QString, vendor);

       QCOMPARE(OpenGLInfoSourcePrivate::normalizeVendor(input.toLocal8Bit().constData()), vendor);
   }

   void testNormalizeRenderer_data()
   {
       QTest::addColumn<QString>("input");
       QTest::addColumn<QString>("renderer");

       QTest::newRow("empty") << QString() << QString();
       QTest::newRow("intel mesa") << QStringLiteral("Mesa DRI Intel(R) HD Graphics 520 (Skylake GT2)") << QStringLiteral("HD Graphics 520 (Skylake GT2)");
       QTest::newRow("intel macOS") << QStringLiteral("Intel Iris Pro OpenGL Engine") << QStringLiteral("Iris Pro");
       QTest::newRow("intel windows") << QStringLiteral("Intel(R) HD Graphics 4600") << QStringLiteral("HD Graphics 4600");
       QTest::newRow("nvidia") << QStringLiteral("GeForce GTX 1060 6GB/PCIe/SSE2") << QStringLiteral("GeForce GTX 1060 6GB/PCIe/SSE2");
       QTest::newRow("nvidia macos") << QStringLiteral("NVIDIA GeForce GT 750M OpenGL Engine") << QStringLiteral("GeForce GT 750M");
       QTest::newRow("angle") << QStringLiteral("ANGLE (VirtualBox Graphics Adapter for Windows 8+ Direct3D9Ex vs_3_0 ps_3_0)") << QStringLiteral("ANGLE");
       QTest::newRow("gallium") << QStringLiteral("Gallium 0.4 on AMD BARTS (DRM 2.49.0 / 4.11.1-1-default, LLVM 4.0.0)") << QStringLiteral("Gallium 0.4 on AMD BARTS");
   }

   void testNormalizeRenderer()
   {
       QFETCH(QString, input);
       QFETCH(QString, renderer);

       QCOMPARE(OpenGLInfoSourcePrivate::normalizeRenderer(input.toLocal8Bit().constData()), renderer);
   }

};

QTEST_MAIN(OpenGLInfoSourceTest)

#include "openglinfosourcetest.moc"
