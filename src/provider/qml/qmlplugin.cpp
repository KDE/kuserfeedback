/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
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

/*!
 * \qmltype ApplicationVersionSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::ApplicationVersionSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype CompilerInfoSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::CompilerInfoSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype CpuInfoSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::CpuInfoSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype LocaleInfoSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::LocaleInfoSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype OpenGLInfoSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::OpenGLInfoSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype PlatformInfoSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::PlatformInfoSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype QPAInfoSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::QPAInfoSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype QtVersionSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::QtVersionSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype ScreenInfoSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::ScreenInfoSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype StartCountSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::StartCountSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype UsageTimeSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::UsageTimeSource
 * \inherits AbstractDataSource
 */

/*!
 * \qmltype AuditLogUiController
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::AuditLogUiController
 */

/*!
 * \qmltype FeedbackConfigUiController
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::FeedbackConfigUiController
 */

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

#include "moc_qmlplugin.cpp"
