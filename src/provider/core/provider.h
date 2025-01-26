/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_PROVIDER_H
#define KUSERFEEDBACK_PROVIDER_H

#include "kuserfeedbackcore_export.h"

#include <QMetaType>
#include <QObject>
#include <QUrl>

namespace KUserFeedback {

class AbstractDataSource;
class ProviderPrivate;
class SurveyInfo;

/*!
 * \class KUserFeedback::Provider
 * \inmodule KUserFeedbackCore
 * \inheaderfile KUserFeedback/Provider
 *
 * \brief The central object managing data sources and transmitting feedback to the server.
 *
 * The defaults for this class are very defensive, so in order to make it actually
 * operational and submit data, there is a number of settings you need to set in
 * code, namely submission intervals, encouragement settings and adding data sources.
 *
 * The settings about what data to submit (telemetryMode) and how often
 * to bother the user with surveys (surveyInterval) should not be set to hardcoded
 * values in code, but left as choices to the user.
 */
class KUSERFEEDBACKCORE_EXPORT Provider : public QObject
{
    Q_OBJECT
    /*!
     * \property KUserFeedback::Provider::enabled
     *
     * The global enabled state of the feedback functionality.
     *
     * If this is \c false, all feedback functionality has to be disabled completely.
     */
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

    /*!
     * \property KUserFeedback::Provider::surveyInterval
     *
     * The interval in which the user accepts surveys.
     *
     * This should be configurable for the user.
     *
     * \c -1 indicates surveys are disabled.
     *
     * \sa surveyInterval(), setSurveyInterval()
     */
    Q_PROPERTY(int surveyInterval READ surveyInterval WRITE setSurveyInterval NOTIFY surveyIntervalChanged)

    /*!
     * \property KUserFeedback::Provider::telemetryMode
     *
     * The telemetry mode the user has configured.
     *
     * This should be configurable for the user.
     *
     * \sa telemetryMode(), setTelemetryMode()
     */
    Q_PROPERTY(TelemetryMode telemetryMode READ telemetryMode WRITE setTelemetryMode NOTIFY telemetryModeChanged)

    /*!
     * \property KUserFeedback::Provider::productIdentifier
     *
     * Unique product id as set on the feedback server.
     *
     * \sa setProductIdentifier
     */
    Q_PROPERTY(QString productIdentifier READ productIdentifier WRITE setProductIdentifier NOTIFY providerSettingsChanged)

    /*!
     * \property KUserFeedback::Provider::feedbackServer
     *
     * URL of the feedback server.
     *
     * \sa setFeedbackServer
     */
    Q_PROPERTY(QUrl feedbackServer READ feedbackServer WRITE setFeedbackServer NOTIFY providerSettingsChanged)

    /*!
     * \property KUserFeedback::Provider::submissionInterval
     *
     * Submission interval in days.
     *
     * \sa setSubmissionInterval
     */
    Q_PROPERTY(int submissionInterval READ submissionInterval WRITE setSubmissionInterval NOTIFY providerSettingsChanged)

    /*!
     * \property KUserFeedback::Provider::applicationStartsUntilEncouragement
     *
     * Times the application has to be started before an encouragement message is shown.
     *
     * \sa setApplicationStartsUntilEncouragement
     */
    Q_PROPERTY(int applicationStartsUntilEncouragement
                READ applicationStartsUntilEncouragement
                WRITE setApplicationStartsUntilEncouragement
                NOTIFY providerSettingsChanged)

    /*!
     * \property KUserFeedback::Provider::applicationUsageTimeUntilEncouragement
     *
     * Application usage time in seconds before an encouragement message is shown.
     *
     * \sa setApplicationUsageTimeUntilEncouragement
     */
    Q_PROPERTY(int applicationUsageTimeUntilEncouragement
                READ applicationUsageTimeUntilEncouragement
                WRITE setApplicationUsageTimeUntilEncouragement
                NOTIFY providerSettingsChanged)

    /*!
     * \property KUserFeedback::Provider::encouragementDelay
     *
     * Encouragement delay after application start in seconds.
     *
     * \sa setEncouragementDelay
     */
    Q_PROPERTY(int encouragementDelay READ encouragementDelay WRITE setEncouragementDelay NOTIFY providerSettingsChanged)

    /*!
     * \property KUserFeedback::Provider::encouragementInterval
     *
     * Encouragement interval.
     *
     * \sa setEncouragementInterval
     */
    Q_PROPERTY(int encouragementInterval READ encouragementInterval WRITE setEncouragementInterval NOTIFY providerSettingsChanged)

    /*!
     * \property KUserFeedback::Provider::describeDataSources
     */
    Q_PROPERTY(QString describeDataSources READ describeDataSources NOTIFY dataSourcesChanged)

public:
    /*!
     * Telemetry collection modes.
     *
     * Collection modes are inclusive, ie. higher modes always imply data from
     * lower modes too.
     *
     * \value NoTelemetry Transmit no data at all.
     * \value BasicSystemInformation Transmit basic information about the* system.
     * \value BasicUsageStatistics Transmit basic usage statistics.
     * \value DetailedSystemInformation Transmit detailed system information.
     * \value DetailedUsageStatistics Transmit detailed usage statistics.
     */
    enum TelemetryMode {
        NoTelemetry,
        BasicSystemInformation = 0x10,
        BasicUsageStatistics = 0x20,
        DetailedSystemInformation = 0x30,
        DetailedUsageStatistics = 0x40,
    };
    Q_ENUM(TelemetryMode)

    /*!
     * Create a new feedback provider.
     *
     * \a parent The parent object.
     */
    explicit Provider(QObject *parent = nullptr);
    ~Provider() override;

    /*!
     * Returns whether feedback functionality is enabled on this system.
     *
     * This should be checked everywhere showing feedback UI to the user
     * to respect the global "kill switch" for this. Provider does check
     * this internally for encouragements, surveys and telemetry submission.
     */
    bool isEnabled() const;

    /*!
     * Set the global (user-wide) activation state for feedback functionality.
     *
     * \sa isEnabled
     */
    void setEnabled(bool enabled);

    /*!
     * Set the telemetry mode and the survey interval back to their default values.
     * \sa telemetryMode(), surveyInterval()
     *
     * \since 1.1.0
     */
    void restoreDefaults();

    /*! Returns the current product identifier. */
    QString productIdentifier() const;

    /*!
     * Set the product identifier.
     *
     * This is used to distinguish independent products on the same server.
     *
     * If this is not specified, the product identifier is derived from the application name
     * organisation domain specified in QCoreApplication.
     *
     * \a productId Unique product identifier, as configured on the feedback server.
     */
    void setProductIdentifier(const QString &productId);

    /*! Returns the current feedback server URL. */
    QUrl feedbackServer() const;

    /*!
     * Set the feedback server URL.
     *
     * This must be called with an appropriate URL for this class to be operational.
     *
     * \a url The URL of the feedback server.
     */
    void setFeedbackServer(const QUrl &url);

    /*!
     * Returns the current submission interval.
     *
     * Returns days between telemetry submissions, or -1 if submission is off.
     */
    int submissionInterval() const;

    /*!
     * Set the automatic submission interval in days.
     *
     * This must be called with a positive number for this class to be operational,
     * as the default is -1 (no submission ever).
     */
    void setSubmissionInterval(int days);

    /*!
     * Returns the current telemetry collection mode.
     *
     * The default is NoTelemetry.
     */
    TelemetryMode telemetryMode() const;

    /*! Set which telemetry data should be submitted. */
    void setTelemetryMode(TelemetryMode mode);

    /*!
     * Adds a data source for telemetry data collection.
     *
     * \a source The data source to add. The Provider takes ownership of \a source.
     */
    void addDataSource(AbstractDataSource *source);

    /*!
     * Returns all data sources that have been added to this provider.
     * \sa addDataSource
     */
    QVector<AbstractDataSource*> dataSources() const;

    /*!
     * Returns a data source with matched \a id
     *
     * \a id data source unique identifier
     *
     * Returns pointer to found data source or nullptr if data source is not found
     */
    AbstractDataSource *dataSource(const QString &id) const;

    /*!
     * Returns the minimum time between two surveys in days.
     *
     * The default is -1 (no surveys enabled).
     */
    int surveyInterval() const;

    /*!
     * Sets the minimum time in days between two surveys.
     *
     * \c -1 indicates no surveys should be requested.
     *
     * \c 0 indicates no minimum time between surveys at all (i.e. bother the user as often as you want).
     */
    void setSurveyInterval(int days);

    /*! Returns the amount of application starts before an encouragement message is shown. */
    int applicationStartsUntilEncouragement() const;

    /*!
     * Set the amount of application starts until the encouragement message should be shown.
     *
     * The default is -1, ie. no encouragement based on application starts.
     *
     * \a starts The amount of application starts after which an encouragement
     * message should be displayed.
     */
    void setApplicationStartsUntilEncouragement(int starts);

    /*! Returns the amount of application usage time before an encouragement message is shown. */
    int applicationUsageTimeUntilEncouragement() const;

    /*!
     * Set the amount of usage time until the encouragement message should be shown.
     *
     * The default is -1, ie. no encouragement based on application usage time.
     *
     * \a secs Amount of seconds until the encouragement should be shown.
     */
    void setApplicationUsageTimeUntilEncouragement(int secs);

    /*! Returns the current encouragement delay in seconds. */
    int encouragementDelay() const;

    /*!
     * Set the delay after application start for the earliest display of the encouragement message.
     *
     * The default is 300, ie. 5 minutes after the application start.
     *
     * \note This only adds an additional constraint on usage time and startup count based
     *  encouragement messages, it does not actually trigger encouragement messages itself.
     *
     * \a secs Amount of seconds after the application start for the earliest display
     * of an encouragement message.
     *
     * \sa setApplicationStartsUntilEncouragement, setApplicationUsageTimeUntilEncouragement
     */
    void setEncouragementDelay(int secs);

    /*! Returns the current encouragement interval. */
    int encouragementInterval() const;

    /*!
     * Sets the interval after the encouragement should be repeated.
     *
     * Encouragement messages are only repeated if no feedback options have been enabled.
     *
     * The default is -1, that is no repeated encouragement at all.
     *
     * \a days Days between encouragement messages, 0 disables repeated encouragements.
     */
    void setEncouragementInterval(int days);

    /*! Returns a string with each source and its enable mode. */
    QString describeDataSources() const;

public Q_SLOTS:
    /*! Manually submit currently recorded data. */
    void submit();

    /*!
     * Marks the given survey as completed. This avoids getting further notification
     * about the same survey.
     */
    void surveyCompleted(const KUserFeedback::SurveyInfo &info);

    /*!
     * Manually load settings of the provider and all added data sources.
     *
     * Automatically invoked after object construction and changing product ID.
     *
     * \note Potentially long operation.
     */
    void load();

    /*!
     * Manually store settings of the provider and all added data sources.
     *
     * Will be automatically invoked upon QCoreApplication::aboutToQuit() signal.
     *
     * \note Potentially long operation.
     */
    void store();

Q_SIGNALS:
    /*!
     * Emitted whenever there is a new survey available that can be presented
     * to the user.
     */
    void surveyAvailable(const KUserFeedback::SurveyInfo &survey);

    /*! Indicate that the encouragement notice should be shown. */
    void showEncouragementMessage();

    /*! Emitted when the survey interval changed. */
    void surveyIntervalChanged();

    /*! Emitted when the telemetry collection mode has changed. */
    void telemetryModeChanged();

    /*! Emitted when any provider setting changed. */
    void providerSettingsChanged();

    /*! Emitted when the global enabled state changed. */
    void enabledChanged();

    /*! Emitted when a data source is added or removed. */
    void dataSourcesChanged();

private:
    friend class ProviderPrivate;
    ProviderPrivate * const d;
    // for UI
    Q_PRIVATE_SLOT(d, QByteArray jsonData(KUserFeedback::Provider::TelemetryMode))
    // for testing
    Q_PRIVATE_SLOT(d, bool selectSurvey(const KUserFeedback::SurveyInfo&))
};

}

Q_DECLARE_METATYPE(KUserFeedback::Provider::TelemetryMode)

#endif // KUSERFEEDBACK_PROVIDER_H
