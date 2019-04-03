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

#include <kuserfeedback_version.h>

#include "helpcontroller.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QProcess>
#include <QStandardPaths>

namespace KUserFeedback {
namespace Console {
struct HelpControllerPrivate
{
    HelpControllerPrivate()
        : proc(nullptr) {}

    void startProcess();
    void sendCommand(const QByteArray &cmd);

    QString assistantPath;
    QString qhcPath;
    QProcess *proc;
};
}}

using namespace KUserFeedback::Console;

void HelpControllerPrivate::startProcess()
{
    if (proc)
        return;

    proc = new QProcess(QCoreApplication::instance());
    proc->setProcessChannelMode(QProcess::ForwardedChannels);
    QObject::connect(proc, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [this]() {
        proc->deleteLater();
        proc = nullptr;
    });
    proc->setProgram(assistantPath);
    proc->setArguments({ QLatin1String("-collectionFile"), qhcPath, QLatin1String("-enableRemoteControl") });
    proc->start();
    proc->waitForStarted();
    sendCommand("expandToc 2;");
}

void HelpControllerPrivate::sendCommand(const QByteArray &cmd)
{
    if (!proc)
        return;
    proc->write(cmd);
}

Q_GLOBAL_STATIC(HelpControllerPrivate, s_helpController)

bool HelpController::isAvailable()
{
    auto d = s_helpController();
    if (!d->assistantPath.isEmpty() && !d->qhcPath.isEmpty())
        return true;

    d->assistantPath = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator() + QStringLiteral("assistant");
    QFileInfo assistFile(d->assistantPath);
    if (!assistFile.isExecutable()) {
        d->assistantPath = QStandardPaths::findExecutable(QStringLiteral("assistant"));
        if (d->assistantPath.isEmpty()) {
            qDebug() << "Qt Assistant not found, help not available.";
            return false;
        }
    }

    const QString qhcPath = QStandardPaths::locate(QStandardPaths::DataLocation, QLatin1String("user-feedback.qhc"));
    if (QFileInfo::exists(qhcPath)) {
        d->qhcPath = qhcPath;
        return true;
    }
    qDebug() << "user-feedback.qhc not found - help not available";
    return false;
}

#define STRINGIFY(x) #x
#define INT2STR(x) STRINGIFY(x)

void HelpController::openContents()
{
    Q_ASSERT(isAvailable());
    auto d = s_helpController();
    d->startProcess();
    d->sendCommand("setSource qthelp://org.kde.UserFeedback." INT2STR(KUSERFEEDBACK_VERSION_MAJOR) "." INT2STR(KUSERFEEDBACK_VERSION_MINOR) "/userfeedback/index.html;syncContents\n");
}

void HelpController::openPage(const QString &page)
{
    Q_ASSERT(isAvailable());
    auto d = s_helpController();
    d->startProcess();
    d->sendCommand(QByteArray("setSource qthelp://org.kde.UserFeedback." INT2STR(KUSERFEEDBACK_VERSION_MAJOR) "." INT2STR(KUSERFEEDBACK_VERSION_MINOR) "/") + page.toUtf8() + ";syncContents\n");
}

#undef STRINGIFY
#undef INT2STR
