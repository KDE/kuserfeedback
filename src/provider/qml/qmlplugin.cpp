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

#include "qmlplugin.h"
#include "qmldatasources.h"
#include "qmlpropertysource.h"
#include "qmlpropertyratiosource.h"
#include "qmlproviderextension.h"

#include <auditloguicontroller.h>
#include <feedbackconfiguicontroller.h>
#include <Provider>
#include <SurveyInfo>

#include <QQmlEngine>

using namespace KUserFeedback;

void QmlPlugin::registerTypes(const char* uri)
{
    qmlRegisterExtendedType<Provider, QmlProviderExtension>(uri, 1, 0, "Provider");

    qmlRegisterUncreatableType<QmlAbstractDataSource>(uri, 1, 0, "AbstractDataSource", QStringLiteral("abstract base class"));
    qmlRegisterType<QmlApplicationVersionSource>(uri, 1, 0, "ApplicationVersionSource");
    qmlRegisterType<QmlCompilerInfoSource>(uri, 1, 0, "CompilerInfoSource");
    qmlRegisterType<QmlCpuInfoSource>(uri, 1, 0, "CpuInfoSource");
    qmlRegisterType<QmlLocaleInfoSource>(uri, 1, 0, "LocaleInfoSource");
    qmlRegisterType<QmlOpenGLInfoSource>(uri, 1, 0, "OpenGLInfoSource");
    qmlRegisterType<QmlPlatformInfoSource>(uri, 1, 0, "PlatformInfoSource");
    qmlRegisterType<QmlQPAInfoSource>(uri, 1, 0, "QPAInfoSource");
    qmlRegisterType<QmlQtVersionSource>(uri, 1, 0, "QtVersionSource");
    qmlRegisterType<QmlScreenInfoSource>(uri, 1, 0, "ScreenInfoSource");
    qmlRegisterType<QmlStartCountSource>(uri, 1, 0, "StartCountSource");
    qmlRegisterType<QmlUsageTimeSource>(uri, 1, 0, "UsageTimeSource");
    qmlRegisterType<QmlPropertySource>(uri, 1, 0, "PropertySource");
    qmlRegisterType<QmlPropertyRatioSource>(uri, 1, 0, "PropertyRatioSource");

    qmlRegisterType<AuditLogUiController>(uri, 1, 0, "AuditLogUiController");
    qmlRegisterType<FeedbackConfigUiController>(uri, 1, 0, "FeedbackConfigUiController");

    qRegisterMetaType<SurveyInfo>();
}
