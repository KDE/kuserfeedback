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
        QString(),
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
