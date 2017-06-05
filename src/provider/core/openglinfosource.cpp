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

#include "openglinfosource.h"

#include <QVariant>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>
#include <QWindow>
#endif

using namespace UserFeedback;

OpenGLInfoSource::OpenGLInfoSource()
    : AbstractDataSource(QStringLiteral("opengl"))
{
}

QString OpenGLInfoSource::description() const
{
    return tr("Information about type, version and vendor of the OpenGL stack.");
}

QVariant OpenGLInfoSource::data()
{
    QVariantMap m;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QOpenGLContext context;
    if (context.create()) {
        QWindow window;
        window.setSurfaceType(QSurface::OpenGLSurface);
        window.create();
        context.makeCurrent(&window);
        QOpenGLFunctions functions(&context);
        m.insert(QStringLiteral("vendor"), QString::fromLocal8Bit(reinterpret_cast<const char*>(functions.glGetString(GL_VENDOR))));
        m.insert(QStringLiteral("renderer"), QString::fromLocal8Bit(reinterpret_cast<const char*>(functions.glGetString(GL_RENDERER))));

        switch (context.openGLModuleType()) {
            case QOpenGLContext::LibGL:
            {
                m.insert(QStringLiteral("type"), QStringLiteral("GL"));

                int major = 0, minor = 0;
                functions.glGetIntegerv(GL_MAJOR_VERSION, &major);
                functions.glGetIntegerv(GL_MINOR_VERSION, &minor);
                m.insert(QStringLiteral("version"), QString(QString::number(major) + QLatin1Char('.') + QString::number(minor)));

                auto vendorVersion = QString::fromLocal8Bit(reinterpret_cast<const char*>(functions.glGetString(GL_VERSION)));
                const auto idx = vendorVersion.indexOf(QLatin1Char(' '));
                if (idx > 0) {
                    vendorVersion = vendorVersion.mid(idx + 1);
                    if (!vendorVersion.isEmpty())
                        m.insert(QStringLiteral("vendorVersion"), vendorVersion);
                }

                break;
            }
            case QOpenGLContext::LibGLES:
            {
                m.insert(QStringLiteral("type"), QStringLiteral("GLES"));

                auto rawVersion = QString::fromLocal8Bit(reinterpret_cast<const char*>(functions.glGetString(GL_VERSION)));
                if (!rawVersion.startsWith(QLatin1String("OpenGL ES "))) {
                    m.insert(QStringLiteral("version"), QStringLiteral("unknown"));
                    m.insert(QStringLiteral("vendorVersion"), rawVersion);
                } else {
                    rawVersion = rawVersion.mid(10);
                    const auto idx = rawVersion.indexOf(QLatin1Char(' '));
                    if (idx > 0) {
                        const auto vendorVersion = rawVersion.mid(idx + 1);
                        if (!vendorVersion.isEmpty())
                            m.insert(QStringLiteral("vendorVersion"), vendorVersion);
                        m.insert(QStringLiteral("version"), rawVersion.left(idx));
                    } else {
                        m.insert(QStringLiteral("version"), rawVersion);
                    }
                }

                break;
            }
        }

        m.insert(QStringLiteral("glslVersion"), QString::fromLocal8Bit(reinterpret_cast<const char*>(functions.glGetString(GL_SHADING_LANGUAGE_VERSION))));

        switch (context.format().profile()) {
            case QSurfaceFormat::NoProfile:
                break;
            case QSurfaceFormat::CoreProfile:
                m.insert(QStringLiteral("profile"), QStringLiteral("core"));
                break;
            case QSurfaceFormat::CompatibilityProfile:
                m.insert(QStringLiteral("profile"), QStringLiteral("compat"));
                break;
        }

        return m;
    }
#endif

    m.insert(QStringLiteral("type"), QStringLiteral("none"));
    return m;
}
