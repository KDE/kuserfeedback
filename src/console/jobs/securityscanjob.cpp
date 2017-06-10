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

#include <kuserfeedback_version.h>

#include "securityscanjob.h"

#include <rest/restclient.h>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace KUserFeedback::Console;

SecurityScanJob::SecurityScanJob(RESTClient* restClient, QObject* parent)
    : Job(parent)
    , m_restClient(restClient)
    , m_pendingPaths({
        QStringLiteral(""),
        QStringLiteral(".htaccess"),
        QStringLiteral("admin"),
        QStringLiteral("admin/index.php"),
        QStringLiteral("analytics"),
        QStringLiteral("analytics/index.php"),
        QStringLiteral("analytics/products"),
        QStringLiteral("config"),
        QStringLiteral("config/localconfig.php"),
        QStringLiteral("data"),
        QStringLiteral("receiver"),
        QStringLiteral("receiver/index.php"),
        QStringLiteral("shared"),
        QStringLiteral("shared/config.php"),
        QStringLiteral("shared/schema.json")
    })
{
    Q_ASSERT(m_restClient);
    Q_ASSERT(m_restClient->isConnected());

    processPending();
}

SecurityScanJob::~SecurityScanJob()
{
}

void SecurityScanJob::processPending()
{
    if (m_pendingPaths.isEmpty()) {
        emit info(tr("No issues found."));
        emitFinished();
        return;
    }

    const auto command = m_pendingPaths.takeFirst();

    auto url = m_restClient->serverInfo().url();
    auto path = url.path();
    if (!path.endsWith(QLatin1Char('/')))
        path += QLatin1Char('/');
    path += command;
    url.setPath(path);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QString(QStringLiteral("UserFeedbackConsole/") + QStringLiteral(KUSERFEEDBACK_VERSION_STRING)));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

    auto reply = m_restClient->networkAccessManager()->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        const auto httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (reply->error() == QNetworkReply::NoError && httpCode < 400) {
            qWarning() << reply->error() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            emitError(tr("Access to %1 is not protected!").arg(reply->request().url().toString()));
            return;
        }
        emit info(tr("Access to %1 is protected (%2).").arg(reply->request().url().toString()).arg(httpCode));
        processPending();
    });
}
