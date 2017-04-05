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

#include <QMetaType>
#include <QObject>
#include <QUrl>

namespace UserFeedback {

class AbstractDataSource;
class ProviderPrivate;
class SurveyInfo;

/*! The central object managing data sources and transmitting feedback to the server. */
class USERFEEDBACKCORE_EXPORT Provider : public QObject
{
    Q_OBJECT
    /*! The interval in which the user accepts surveys.
     *  @c -1 indicates surveys are disabled.
     *  @see surveyInterval(), setSurveyInterval()
     */
    Q_PROPERTY(int surveyInterval READ surveyInterval WRITE setSurveyInterval NOTIFY surveyIntervalChanged)
    /*! The telemetry mode the user has configured.
     * @see statisticsCollectionMode(), setStatisticsCollectionMode()
     */
    Q_PROPERTY(StatisticsCollectionMode statisticsCollectionMode READ statisticsCollectionMode WRITE setStatisticsCollectionMode NOTIFY statisticsCollectionModeChanged)
public:
    /*! Telemetry collection modes.
     *  Colleciton modes are inclusive, ie. higher modes always imply data from
     *  lower modes too.
     */
    enum StatisticsCollectionMode {
        NoStatistics, ///< Transmit no data at all.
        BasicSystemInformation = 0x10, ///< Transmit basic information about the system.
        BasicUsageStatistics = 0x20, ///< Transmit basic usage statistics.
        DetailedSystemInformation = 0x30, ///< Transmit detailed system information.
        DetailedUsageStatistics = 0x40, ///< Transmit detailed usage statistics.
    };
    Q_ENUMS(StatisticsCollectionMode)

    /*! Create a new feedback provider.
     *  @param parent The parent object.
     */
    explicit Provider(QObject *parent = nullptr);
    ~Provider();

    /*! Set the product identifier.
     *  This is used to distinguish independent products on the same server.
     *  @param productId Unique product identifier, as configured on the feedback server.
     */
    void setProductIdentifier(const QString &productId);

    /*! Set the feedback server URL.
     *  @param url The URL of the feedback server.
     */
    void setFeedbackServer(const QUrl &url);

    /*! Set the automatic submission interval. */
    void setSubmissionInterval(int days);

    /*! Returns the current statistics collection mode. */
    StatisticsCollectionMode statisticsCollectionMode() const;

    /*! Set which statistics should be submitted. */
    void setStatisticsCollectionMode(StatisticsCollectionMode mode);

    /*! Adds a data source for statistical data collection.
     *  @param source The data source to add. The Provider takes ownership of @p source.
     *  @param mode The statistics collection mode this source belongs to. Data is only
     *  send to the server for this source is a sufficiently high collection mode is configured
     *  by the user. @c NoStatistics is not allowed.
     */
    void addDataSource(AbstractDataSource *source, StatisticsCollectionMode mode);

    /*! Returns all data sources that have been added to this provider.
     *  @see addDataSource
     */
    QVector<AbstractDataSource*> dataSources() const;

    /*! Returns the minimum time between two surveys in days. */
    int surveyInterval() const;

    /*! Sets the minimum time in days between two surveys.
     *  @c -1 indicates no surveys should be requested.
     */
    void setSurveyInterval(int days);

    /*! Marks the given survey as completed. This avoids getting further notification
     *  about the same survey.
     */
    void setSurveyCompleted(const SurveyInfo &info);

    /*! Set the amount of application starts until the encouragement message should be shown. */
    void setApplicationStartsUntilEncouragement(int starts);

    /*! Set the amount of usage time until the encouragement message should be shown.
     *  @param secs Amount of seconds until the encouragement should be shown.
     */
    void setApplicationUsageTimeUntilEncouragement(int secs);

    /*! Set the delay after application start for the earliest display of the encouragement message. */
    void setEncouragementDelay(int secs);

    /*! Sets the interval after the encouragement should be repeated.
     *  Encouragement messages are only repeated if no feedback options have been enabled.
     *  @param days Days between encouragement messages, 0 disables repeated encouragements.
     */
    void setEncouragementInterval(int days);

public slots:
    /*! Manually submit currently recorded data. */
    void submit();

signals:
    /*! Emitted whenever there is a new survey available that can be presented
     *  to the user.
     */
    void surveyAvailable(const UserFeedback::SurveyInfo &survey);

    /*! Indicate that the encouragement notice should be shown. */
    void showEncouragementMessage();

    /*! Emitted when the survey interval changed. */
    void surveyIntervalChanged();

    /*! Emitted when the statistics collection mode has changed. */
    void statisticsCollectionModeChanged();

private:
    friend class ProviderPrivate;
    ProviderPrivate * const d;
    Q_PRIVATE_SLOT(d, void aboutToQuit())
    Q_PRIVATE_SLOT(d, void submitFinished())
    Q_PRIVATE_SLOT(d, void emitShowEncouragementMessage())
    // for UI
    Q_PRIVATE_SLOT(d, QByteArray jsonData(UserFeedback::Provider::StatisticsCollectionMode))
    // for testing
    Q_PRIVATE_SLOT(d, void load())
    Q_PRIVATE_SLOT(d, void store())
};

}

Q_DECLARE_METATYPE(UserFeedback::Provider::StatisticsCollectionMode)

#endif // USERFEEDBACK_PROVIDER_H
