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

#ifndef USERFEEDBACK_PROVIDER_H
#define USERFEEDBACK_PROVIDER_H

#include "userfeedbackcore_export.h"

#include <QObject>
#include <QUrl>


namespace UserFeedback {

class ProviderPrivate;
class SurveyInfo;

/** The central object managing data sources and transmitting feedback to the server. */
class USERFEEDBACKCORE_EXPORT Provider : public QObject
{
    Q_OBJECT
public:
    enum StatisticsCollectionMode {
        NoStatistics,
        BasicStatistics,
        AllStatistics
    };
    Q_ENUMS(StatisticsCollectionMode)

    explicit Provider(QObject *parent = Q_NULLPTR);
    ~Provider();

    /** Set the product identifier.
     *  This is used to distinguish independent products on the same server.
     */
    void setProductIdentifier(const QString &productId);

    /** Set the feedback server URL. */
    void setFeedbackServer(const QUrl &url);

    /** Set the automatic submission interval. */
    void setSubmissionInterval(int days);

    /** Returns the current statistics collection mode. */
    StatisticsCollectionMode statisticsCollectionMode() const;

    /** Set which statistics should be submitted. */
    void setStatisticsCollectionMode(StatisticsCollectionMode mode);

    /** Returns the minimum time between two surveys in days. */
    int surveyInterval() const;

    /** Sets the minimum time in days between two surveys.
     *  @c -1 indicates no surveys should be requested.
     */
    void setSurveyInterval(int days);

    /** Marks the given survey as completed. This avoids getting further notification
     *  about the same survey.
     */
    void setSurveyCompleted(const SurveyInfo &info);

    /** Set the amount of application starts until the encouragement message should be shown. */
    void setApplicationStartsUntilEncouragement(int starts);

    /** Set the amount of usage time until the encouragement message should be shown. */
    void setApplicationUsageTimeUntilEncouragement(int minutes);

    /** Set the delay after application start for the earliest display of the encouragement message. */
    void setEncouragementDelay(int secs);

public slots:
    /** Manually submit currently recorded data. */
    void submit();

signals:
    /** Emitted whenever there is a new survey available that can be presented
     *  to the user.
     */
    void surveyAvailable(const UserFeedback::SurveyInfo &survey);

    /** Indicate that the encouragement notice should be shown. */
    void showEncouragementMessage();

private:
    friend class ProviderPrivate;
    ProviderPrivate * const d;
    Q_PRIVATE_SLOT(d, void aboutToQuit())
    Q_PRIVATE_SLOT(d, void submitFinished())
    Q_PRIVATE_SLOT(d, void emitShowEncouragementMessage())
    // for testing
    Q_PRIVATE_SLOT(d, void load())
    Q_PRIVATE_SLOT(d, void store())
};

}

#endif // USERFEEDBACK_PROVIDER_H
