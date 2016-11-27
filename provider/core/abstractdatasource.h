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

#ifndef USERFEEDBACK_ABSTRACTDATASOURCE_H
#define USERFEEDBACK_ABSTRACTDATASOURCE_H

#include "userfeedbackcore_export.h"
#include "provider.h"

class QSettings;

namespace UserFeedback {

class AbstractDataSourcePrivate;

/** Base class for data sources for statistical data. */
class USERFEEDBACKCORE_EXPORT AbstractDataSource
{
public:
    virtual ~AbstractDataSource();

    /** Returns the name of this data source. */
    QString name() const;

    /** Implement this to return the data gathered by this source.
     *  Depending on the schema entry type for this source, the following
     *  formats are expected:
     *  - scalar entries: QAssiciativeIterable
     *  - list entries: QSequentialIterable containing QAssociativeIterable
     *  - map entries: QAssiciativeIterable containing QAssociativeIterable
     * The innermost QAssiciativeIterable must only contain one of the following
     * base types (which has to match the corresponding schema entry element):
     * - QString
     * - int
     * - double
     * - bool
     * All keys must be strings.
     */
    virtual QVariant data() = 0;

    /** Load persistent state for this data source. */
    virtual void load(QSettings *settings);

    /** Store persistent state for this data source. */
    virtual void store(QSettings *settings);

    /** Returns which colleciton mode this data source belongs to. */
    Provider::StatisticsCollectionMode collectionMode() const;
    /** Sets which colleciton mode this data source belongs to. */
    void setCollectionMode(Provider::StatisticsCollectionMode mode);

protected:
    /** Create a new data source named @p name.
     *  The name of the data source must match the corresponding
     *  product schema entry.
     *  @param name Must not be empty.
     */
    explicit AbstractDataSource(const QString &name, AbstractDataSourcePrivate *dd = Q_NULLPTR);
    class AbstractDataSourcePrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractDataSource)
    Q_DISABLE_COPY(AbstractDataSource)
};
}

#endif // USERFEEDBACK_ABSTRACTDATASOURCE_H
