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

#include <config-userfeedback-version.h>

#include "provider.h"
#include "provider_p.h"
#include "abstractdatasource.h"
#include "startcountsource.h"
#include "surveyinfo.h"
#include "usagetimesource.h"

#include <QCoreApplication>
#include <QDebug>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#endif
#include <QMetaEnum>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>

#include <algorithm>
#include <numeric>

using namespace UserFeedback;

ProviderPrivate::ProviderPrivate(Provider *qq)
    : q(qq)
    , networkAccessManager(nullptr)
    , submissionInterval(-1)
    , statisticsMode(Provider::NoStatistics)
    , surveyInterval(-1)
    , startCount(0)
    , usageTime(0)
    , encouragementStarts(-1)
    , encouragementTime(-1)
    , encouragementDelay(300)
    , encouragementDisplayed(false)
{
    auto domain = QCoreApplication::organizationDomain().split(QLatin1Char('.'));
    std::reverse(domain.begin(), domain.end());
    productId = domain.join(QLatin1String(".")) + QLatin1Char('.') + QCoreApplication::applicationName();

    submissionTimer.setSingleShot(true);
    QObject::connect(&submissionTimer, SIGNAL(timeout()), q, SLOT(submit()));

    startTime.start();

    encouragementTimer.setSingleShot(true);
    QObject::connect(&encouragementTimer, SIGNAL(timeout()), q, SLOT(emitShowEncouragementMessage()));
}

ProviderPrivate::~ProviderPrivate()
{
    qDeleteAll(dataSources);
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

static QMetaEnum statisticsCollectionModeEnum()
{
    const auto idx = Provider::staticMetaObject.indexOfEnumerator("StatisticsCollectionMode");
    Q_ASSERT(idx >= 0);
    return Provider::staticMetaObject.enumerator(idx);
}

void ProviderPrivate::load()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("UserFeedback"));
    lastSubmitTime = settings.value(QStringLiteral("LastSubmission")).toDateTime();

    const auto modeStr = settings.value(QStringLiteral("StatisticsCollectionMode")).toByteArray();
    statisticsMode = static_cast<Provider::StatisticsCollectionMode>(std::max(statisticsCollectionModeEnum().keyToValue(modeStr), 0));

    surveyInterval = settings.value(QStringLiteral("SurveyInterval"), -1).toInt();
    lastSurveyTime = settings.value(QStringLiteral("LastSurvey")).toDateTime();
    completedSurveys = settings.value(QStringLiteral("CompletedSurveys"), QStringList()).toStringList();

    startCount = std::max(settings.value(QStringLiteral("ApplicationStartCount"), 0).toInt() + 1, 1);
    usageTime = std::max(settings.value(QStringLiteral("ApplicationTime"), 0).toInt(), 0);

    encouragementDisplayed = settings.value(QStringLiteral("EncouragementDisplayed"), false).toBool();
}

void ProviderPrivate::store()
{
    QSettings settings;
    settings.beginGroup(QStringLiteral("UserFeedback"));
    settings.setValue(QStringLiteral("LastSubmission"), lastSubmitTime);
    settings.setValue(QStringLiteral("StatisticsCollectionMode"), QString::fromLatin1(statisticsCollectionModeEnum().valueToKey(statisticsMode)));

    settings.setValue(QStringLiteral("SurveyInterval"), surveyInterval);
    settings.setValue(QStringLiteral("LastSurvey"), lastSurveyTime);
    settings.setValue(QStringLiteral("CompletedSurveys"), completedSurveys);

    settings.setValue(QStringLiteral("ApplicationStartCount"), startCount);
    settings.setValue(QStringLiteral("ApplicationTime"), currentApplicationTime());

    settings.setValue(QStringLiteral("EncouragementDisplayed"), encouragementDisplayed);

    foreach (auto source, dataSources)
        source->store(&settings);
}

void ProviderPrivate::aboutToQuit()
{
    qDebug() << Q_FUNC_INFO;
    store();
}

QByteArray ProviderPrivate::jsonData() const
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QJsonObject obj;
    if (statisticsMode != Provider::NoStatistics) {
        foreach (auto source, dataSources) {
            if (statisticsMode < source->collectionMode())
                continue;
            const auto data = source->data();
            if (data.canConvert<QVariantMap>())
                obj.insert(source->name(), QJsonObject::fromVariantMap(data.toMap()));
            else if (data.canConvert<QVariantList>())
                obj.insert(source->name(), QJsonArray::fromVariantList(data.value<QVariantList>()));
        }
    }

    QJsonDocument doc(obj);
    return doc.toJson();
#else
    qCritical("NOT IMPLEMENTED YET");
    return QByteArray();
#endif
}

void ProviderPrivate::scheduleNextSubmission()
{
    submissionTimer.stop();
    if (submissionInterval <= 0 || (statisticsMode == Provider::NoStatistics && surveyInterval < 0))
        return;

    Q_ASSERT(submissionInterval > 0);

    const auto nextSubmission = lastSubmitTime.addDays(submissionInterval);
    const auto now = QDateTime::currentDateTime();
    submissionTimer.start(std::max(0ll, now.msecsTo(nextSubmission)));
}

void ProviderPrivate::submitFinished()
{
    auto reply = qobject_cast<QNetworkReply*>(q->sender());
    Q_ASSERT(reply);

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "failed to submit user feedback:" << reply->errorString() << reply->readAll();
        return;
    }

    lastSubmitTime = QDateTime::currentDateTime();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    const auto obj = QJsonDocument::fromJson(reply->readAll()).object();
    if (obj.contains(QStringLiteral("survey"))) {
        const auto surveyObj = obj.value(QStringLiteral("survey")).toObject();
        const auto survey = SurveyInfo::fromJson(surveyObj);
        selectSurvey(survey);
    }
#endif

    scheduleNextSubmission();
}

void ProviderPrivate::selectSurvey(const SurveyInfo &survey) const
{
    qDebug() << Q_FUNC_INFO << "got survey:" << survey.url();
    if (surveyInterval < 0) // surveys disabled
        return;

    if (!survey.isValid() || completedSurveys.contains(QString::number(survey.id())))
        return;

    if (lastSurveyTime.addDays(surveyInterval) > QDateTime::currentDateTime())
        return;

    emit q->surveyAvailable(survey);
}

void ProviderPrivate::scheduleEncouragement()
{
    encouragementTimer.stop();
    if (encouragementStarts < 0 && encouragementTime < 0) // encouragement disabled
        return;

    if (encouragementStarts > startCount) // we need more starts
        return;

    if (statisticsMode == Provider::AllStatistics && surveyInterval == 0) // already everything enabled
        return;

    Q_ASSERT(encouragementDelay >= 0);
    int timeToEncouragement = encouragementDelay;
    if (encouragementTime > 0)
        timeToEncouragement = std::max(timeToEncouragement, (encouragementTime * 60) - currentApplicationTime());
    encouragementTimer.start(timeToEncouragement);
}

void ProviderPrivate::emitShowEncouragementMessage()
{
    encouragementDisplayed = true; // TODO make this explicit, in case the host application decides to delay?
    emit q->showEncouragementMessage();
}


Provider::Provider(QObject *parent) :
    QObject(parent),
    d(new ProviderPrivate(this))
{
    qDebug() << Q_FUNC_INFO;

    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));

    d->load();
}

Provider::~Provider()
{
    delete d;
}

void Provider::setProductIdentifier(const QString &productId)
{
    d->productId = productId;
}

void Provider::setFeedbackServer(const QUrl &url)
{
    d->serverUrl = url;
}

void Provider::setSubmissionInterval(int days)
{
    d->submissionInterval = days;
    d->scheduleNextSubmission();
}

Provider::StatisticsCollectionMode Provider::statisticsCollectionMode() const
{
    return d->statisticsMode;
}

void Provider::setStatisticsCollectionMode(StatisticsCollectionMode mode)
{
    if (d->statisticsMode == mode)
        return;

    d->statisticsMode = mode;
    d->scheduleNextSubmission();
    d->scheduleEncouragement();
    d->store();
    emit statisticsCollectionModeChanged();
}

void Provider::addDataSource(AbstractDataSource *source, StatisticsCollectionMode mode)
{
    // sanity-check sources
    if (mode == NoStatistics) {
        qCritical() << "Source" << source->name() << "attempts to report data unconditionally, ignoring!";
        delete source;
        return;
    }
    if (source->description().isEmpty()) {
        qCritical() << "Source" << source->name() << "has no description, ignoring!";
        delete source;
        return;
    }

    Q_ASSERT(mode != NoStatistics);
    Q_ASSERT(!source->description().isEmpty());
    source->setCollectionMode(mode);

    // special cases for sources where we track the data here, as it's needed even if we don't report it
    if (auto countSrc = dynamic_cast<StartCountSource*>(source))
        countSrc->setProvider(d);
    if (auto timeSrc = dynamic_cast<UsageTimeSource*>(source))
        timeSrc->setProvider(d);

    d->dataSources.push_back(source);

    QSettings settings;
    settings.beginGroup(QStringLiteral("UserFeedback"));
    source->load(&settings);
}

QVector<AbstractDataSource*> Provider::dataSources() const
{
    return d->dataSources;
}

int Provider::surveyInterval() const
{
    return d->surveyInterval;
}

void Provider::setSurveyInterval(int days)
{
    if (d->surveyInterval == days)
        return;

    d->surveyInterval = days;
    d->scheduleNextSubmission();
    d->scheduleEncouragement();
    d->store();
    emit surveyIntervalChanged();
}

void Provider::setApplicationStartsUntilEncouragement(int starts)
{
    d->encouragementStarts = starts;
    d->scheduleEncouragement();
}

void Provider::setApplicationUsageTimeUntilEncouragement(int minutes)
{
    d->encouragementTime = minutes;
    d->scheduleEncouragement();
}

void Provider::setEncouragementDelay(int secs)
{
    d->encouragementDelay = std::max(0, secs);
    d->scheduleEncouragement();
}

void Provider::setSurveyCompleted(const SurveyInfo &info)
{
    d->completedSurveys.push_back(QString::number(info.id()));
    d->lastSurveyTime = QDateTime::currentDateTime();
    d->store();
}

void Provider::submit()
{
    if (!d->serverUrl.isValid()) {
        qWarning() << "No feedback server URL specified!";
        return;
    }

    if (!d->networkAccessManager)
        d->networkAccessManager = new QNetworkAccessManager(this);

    auto url = d->serverUrl;
    url.setPath(url.path() + QStringLiteral("/receiver/submit/") + d->productId);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("UserFeedback/") + QStringLiteral(USERFEEDBACK_VERSION));
#endif
    auto reply = d->networkAccessManager->post(request, d->jsonData());
    connect(reply, SIGNAL(finished()), this, SLOT(submitFinished()));
}

#include "moc_provider.cpp"
