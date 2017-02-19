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

#ifndef USERFEEDBACK_CONSOLE_SCHEMAENTRY_H
#define USERFEEDBACK_CONSOLE_SCHEMAENTRY_H

#include <QObject>
#include <QSharedDataPointer>
#include <QVector>

class QJsonArray;
class QJsonObject;

namespace UserFeedback {
namespace Console {

class SchemaEntryData;
class SchemaEntryElement;

/** Represents one schema entry.
 *  A schema entry can be a scalar, a list or a map, and consists of one or
 *  more named entries. A schema entry can also have higher-level aggregation
 *  hints, used for visualizing the recorded data. The following examples
 *  list common configurations.
 *
 *  \li Product version, Qt version, framework version:
 *  scalar, one string element containing the version, category aggregation
 *  \li Usage time, start count:
 *  scalar, one integer element containing the data, numeric aggregation
 *  \li Platform type/version:
 *  scalar, two string elements containing platform type/sub-type, hierarchical category aggregation
 *  \li View usage/feature usage ratios:
 *  Option 1 (fixed set): scalar, multiple numeric elements per feature, ratio set aggregation
 *  Option 2 (variable set): map for feature to single numeric element, ratio set aggregation
 *  \li Screen size:
 *  list (one entry per screen), two numeric elements for height/width, x/y aggregation
 *  \li Screen resolution, screen count:
 *  scalar of single numeric element, numeric aggregation
 *  \li event counts:
 *  scalar, multiple numeric elements, numeric aggregation
 *  \li language settings:
 *  scalar, one string element, category aggregation
 */
class SchemaEntry
{
    Q_GADGET
public:
    enum DataType {
        Scalar,
        List,
        Map
    };
    Q_ENUM(DataType)

    SchemaEntry();
    SchemaEntry(const SchemaEntry &entry);
    ~SchemaEntry();
    SchemaEntry& operator=(const SchemaEntry &entry);

    bool operator==(const SchemaEntry &other) const;

    int internalId() const;
    void setInternalId(int internalId);

    QString name() const;
    void setName(const QString& name);

    DataType dataType() const;
    void setDataType(DataType type);

    QVector<SchemaEntryElement> elements() const;
    void setElements(const QVector<SchemaEntryElement> &elements);
    SchemaEntryElement element(const QString &name) const;

    QJsonObject toJsonObject() const;
    static QVector<SchemaEntry> fromJson(const QJsonArray &array);

private:
    QSharedDataPointer<SchemaEntryData> d;
};

}
}

Q_DECLARE_METATYPE(UserFeedback::Console::SchemaEntry)
Q_DECLARE_TYPEINFO(UserFeedback::Console::SchemaEntry, Q_MOVABLE_TYPE);

#endif // USERFEEDBACK_CONSOLE_PRODUCTSCHEMAENTRY_H
