/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#include "job.h"

using namespace KUserFeedback::Console;

Job::Job(QObject* parent)
    : QObject(parent)
{
}

Job::~Job() = default;

void Job::emitError(const QString& msg)
{
    emit error(msg);
    deleteLater();
}

void Job::emitFinished()
{
    emit finished();
    deleteLater();
}
