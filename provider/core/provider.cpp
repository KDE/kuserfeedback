/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include "provider.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QTime>
#include <QUrl>

#include <numeric>


using namespace UserFeedback;

namespace UserFeedback {
class ProviderPrivate
{
public:
    ProviderPrivate(Provider *qq);

    void reset();
    int currentApplicationTime() const;

    void load();
    void store();

    void aboutToQuit();

    QByteArray jsonData() const;
    void submitFinished();

    Provider *q;

    QString productId;

    QNetworkAccessManager *networkAccessManager;
    QUrl serverUrl;
    QDateTime lastSubmitTime;

    QTime startTime;
    int startCount;
    int usageTime;
};
}

ProviderPrivate::ProviderPrivate(Provider *qq)
    : q(qq)
    , networkAccessManager(Q_NULLPTR)
    , startCount(0)
    , usageTime(0)
{
    startTime.start();
}

void ProviderPrivate::reset()
{
    startCount = 0;
    usageTime = 0;
    startTime.start();
}

int ProviderPrivate::currentApplicationTime() const
{
    return usageTime + (startTime.elapsed() / 1000);
}

void ProviderPrivate::load()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("UserFeedback"));
    lastSubmitTime = settings.value(QStringLiteral("LastSubmission")).toDateTime();
    startCount = std::max(settings.value(QStringLiteral("ApplicationStartCount"), 0).toInt() + 1, 1);
    usageTime = std::max(settings.value(QStringLiteral("ApplicationTime"), 0).toInt(), 0);
}

void ProviderPrivate::store()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("UserFeedback"));
    settings.setValue(QStringLiteral("LastSubmission"), lastSubmitTime);
    settings.setValue(QStringLiteral("ApplicationStartCount"), startCount);
    settings.setValue(QStringLiteral("ApplicationTime"), currentApplicationTime());
}

void ProviderPrivate::aboutToQuit()
{
    qDebug() << Q_FUNC_INFO;
    store();
}

QByteArray ProviderPrivate::jsonData() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("productId"), productId);
    obj.insert(QStringLiteral("startCount"), startCount);
    obj.insert(QStringLiteral("usageTime"), currentApplicationTime());

    QJsonDocument doc(obj);
    return doc.toJson();
}

void ProviderPrivate::submitFinished()
{
    auto reply = qobject_cast<QNetworkReply*>(q->sender());
    Q_ASSERT(reply);

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "failed to submit user feedback:" << reply->errorString();
        return;
    }

    lastSubmitTime = QDateTime::currentDateTime();

    const auto obj = QJsonDocument::fromJson(reply->readAll()).object();
    if (obj.contains(QStringLiteral("survey"))) {
        const auto surveyObj = obj.value(QStringLiteral("survey")).toObject();
        qDebug() << "got survey: " << surveyObj.value(QStringLiteral("url"));
    }
}


Provider::Provider(QObject *parent) :
    QObject(parent),
    d(new ProviderPrivate(this))
{
    qDebug() << Q_FUNC_INFO;

    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));

    d->load();
}

void Provider::setProductIdentifier(const QString &productId)
{
    d->productId = productId;
}

void Provider::setFeedbackServer(const QUrl &url)
{
    d->serverUrl = url;
}

void Provider::submit()
{
    if (!d->networkAccessManager)
        d->networkAccessManager = new QNetworkAccessManager(this);

    auto url = d->serverUrl;
    url.setPath(url.path() + QStringLiteral("/receiver/submit"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    auto reply = d->networkAccessManager->post(request, d->jsonData());
    connect(reply, SIGNAL(finished()), this, SLOT(submitFinished()));
}

#include "moc_provider.cpp"
