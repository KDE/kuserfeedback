/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_ABSTRACTDATASOURCE_H
#define KUSERFEEDBACK_ABSTRACTDATASOURCE_H

#include "kuserfeedbackcore_export.h"
#include "provider.h"

#include <QCoreApplication>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace KUserFeedback {

class AbstractDataSourcePrivate;

/*!
 * \class KUserFeedback::AbstractDataSource
 * \inmodule KUserFeedbackCore
 * \inheaderfile KUserFeedback/AbstractDataSource
 *
 * \brief Base class for data sources for telemetry data.
 */
class KUSERFEEDBACKCORE_EXPORT AbstractDataSource
{
public:
    virtual ~AbstractDataSource();

    /*!
     * Returns the ID of this data source.
     * This is used as identifier towards the server and should
     * not be shown to the user.
     *
     * Returns the data source identifier configured on the feedback server.
     * \sa description()
     */
    QString id() const;

    /*!
     * Returns a short name of this data source.
     *
     * Can be empty if short name is meaningless for this data source.
     *
     * Returns a translated, human-readable string.
     */
    virtual QString name() const;

    /*!
     * Returns a human-readable, translated description of what
     * this source provides.
     *
     * Returns a translated, human-readable string.
     *
     * \sa id()
     */
    virtual QString description() const = 0;

    /*!
     * Returns the data gathered by this source.
     *
     * Implement this to return the data provided by this source.
     * One of the three following formats are expected:
     * \list
     *  \li scalar entries: QAssociativeIterable
     *  \li list entries: QSequentialIterable containing QAssociativeIterable
     *  \li map entries: QAssociativeIterable containing QAssociativeIterable
     * \endlist
     *
     * The innermost QAssociativeIterable must only contain one of the following
     * base types (which has to match the corresponding schema entry element):
     * \list
     *  \li QString
     *  \li int
     *  \li double
     *  \li bool
     * \endlist
     *
     * All keys must be strings.
     *
     * Returns a variant complying with the above requirements.
     */
    virtual QVariant data() = 0;

    /*!
     * Load persistent state for this data source.
     *
     * \a settings A QSettings object opened in a dedicated group for loading
     * persistent data.
     */
    void load(QSettings *settings);

    /*!
     * Store persistent state for this data source.
     *
     * \a settings A QSettings object opened in a dedicated group for storing
     * persistent data.
     */
    void store(QSettings *settings);

    /*!
     * Reset the persistent state of this data source.
     *
     * This is called after a successful submission of data, and can be used
     * by sources that track differential rather than absolute data to reset
     * their counters.
     *
     * \a settings A QSettings object opened in the dedicated group of this
     * data source.
     */
    void reset(QSettings *settings);

    /*!
     * Returns which telemetry colleciton mode this data source belongs to.
     */
    Provider::TelemetryMode telemetryMode() const;

    /*!
     * Sets which telemetry colleciton mode this data source belongs to.
     *
     * \a mode The data collection mode of this source.
     *
     * Provider::NoTelemetry is not allowed here.
     */
    void setTelemetryMode(Provider::TelemetryMode mode);

    /*!
     * Checks whether this data source is active or not
     *
     * If the data source is not active, then collected
     * data neither will be sent to a server nor appeared
     * in the audit log.
     *
     * Data source is active by default.
     *
     * Returns \c true if active, false otherwise
     */
    bool isActive() const;

    /*!
     * Changes active state of the data source
     *
     * \a active The new active state for this data source
     */
    void setActive(bool active);

protected:
    /*!
     * Create a new data source named \a name.
     *
     * The name of the data source must match the corresponding
     * product schema entry.
     *
     * \a id Must not be empty.
     *
     * \a mode The default telemetry mode.
     */
    explicit AbstractDataSource(const QString &id,
                                Provider::TelemetryMode mode = Provider::DetailedUsageStatistics);

    /*!
     *
     */
    explicit AbstractDataSource(const QString &id,
                                Provider::TelemetryMode mode,
                                AbstractDataSourcePrivate *dd);

    /*!
     * Set the ID of this data source.
     *
     * The ID should not change at runtime, this is only provided
     * for enabling QML API for generic sources.
     *
     * \sa id()
     */
    void setId(const QString &id);

    /*!
     * Invoked by load() in order to load individual settings of this data source.
     *
     * \sa load() description for further details.
     *
     * \a settings A QSettings object opened in a dedicated group for loading
     * persistent data.
     */
    virtual void loadImpl(QSettings *settings);

    /*!
     * Invoked by store() in order to store individual settings of this data source.
     *
     * \sa store() description for further details.
     *
     * \a settings A QSettings object opened in a dedicated group for loading
     * persistent data.
     */
    virtual void storeImpl(QSettings *settings);

    /*!
     * Invoked by reset() in order to reset individual settings of this data source.
     *
     * See reset() description for further details.
     *
     * \a settings A QSettings object opened in a dedicated group for loading
     * persistent data.
     */
    virtual void resetImpl(QSettings *settings);

    class AbstractDataSourcePrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractDataSource)
    Q_DISABLE_COPY(AbstractDataSource)
};
}

#endif // KUSERFEEDBACK_ABSTRACTDATASOURCE_H
