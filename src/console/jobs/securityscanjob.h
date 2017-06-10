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

#ifndef KUSERFEEDBACK_CONSOLE_SECURITYSCANJOB_H
#define KUSERFEEDBACK_CONSOLE_SECURITYSCANJOB_H

#include "job.h"

#include <QVector>

namespace KUserFeedback {
namespace Console {

class RESTClient;

/*! Scan a server for common misconfigurations. */
class SecurityScanJob : public Job
{
    Q_OBJECT
public:
    explicit SecurityScanJob(RESTClient *restClient, QObject *parent = nullptr);
    ~SecurityScanJob();

private:
    void processPending();

    RESTClient *m_restClient;
    QVector<QString> m_pendingPaths;
};

}}

#endif // KUSERFEEDBACK_CONSOLE_SECURITYSCANJOB_H
