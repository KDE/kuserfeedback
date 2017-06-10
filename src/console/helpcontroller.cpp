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

#include <config-userfeedback-version.h>

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

void HelpController::openContents()
{
    Q_ASSERT(isAvailable());
    auto d = s_helpController();
    d->startProcess();
    d->sendCommand("setSource qthelp://org.kde.UserFeedback." USERFEEDBACK_VERSION_MAJOR_MINOR "/userfeedback/index.html;syncContents\n");
}

void HelpController::openPage(const QString &page)
{
    Q_ASSERT(isAvailable());
    auto d = s_helpController();
    d->startProcess();
    d->sendCommand(QByteArray("setSource qthelp://org.kde.UserFeedback." USERFEEDBACK_VERSION_MAJOR_MINOR "/") + page.toUtf8() + ";syncContents\n");
}
