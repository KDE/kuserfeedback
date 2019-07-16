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

#include "qmlplugin.h"
#include "qmldatasources.h"
#include "qmlpropertysource.h"
#include "qmlpropertyratiosource.h"
#include "qmlproviderextension.h"

#include <auditloguicontroller.h>
#include <feedbackconfiguicontroller.h>
#include <provider.h>
#include <surveyinfo.h>

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
