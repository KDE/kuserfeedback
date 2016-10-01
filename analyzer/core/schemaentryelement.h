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

#ifndef USERFEEDBACK_ANALYZER_SCHEMAENTRYELEMENT_H
#define USERFEEDBACK_ANALYZER_SCHEMAENTRYELEMENT_H

#include <qobjectdefs.h>
#include <QMetaType>
#include <QSharedDataPointer>

QT_BEGIN_NAMESPACE
class QJsonArray;
class QJsonObject;
template <typename T> class QVector;
QT_END_NAMESPACE

namespace UserFeedback {
namespace Analyzer {

class SchemaEntryElementData;

/** One element in a SchemaEntry. */
class SchemaEntryElement
{
    Q_GADGET
public:
    enum Type {
        Integer,
        Number,
        String,
        Boolean
    };
    Q_ENUM(Type)

    SchemaEntryElement();
    SchemaEntryElement(const SchemaEntryElement &other);
    ~SchemaEntryElement();
    SchemaEntryElement& operator=(const SchemaEntryElement &other);

    bool operator==(const SchemaEntryElement &other) const;

    QString name() const;
    void setName(const QString& name);

    Type type() const;
    void setType(Type type);

    QJsonObject toJsonObject() const;
    static QVector<SchemaEntryElement> fromJson(const QJsonArray &array);

private:
    QSharedDataPointer<SchemaEntryElementData> d;
};
}
}

Q_DECLARE_METATYPE(UserFeedback::Analyzer::SchemaEntryElement)
Q_DECLARE_METATYPE(UserFeedback::Analyzer::SchemaEntryElement::Type)
Q_DECLARE_TYPEINFO(UserFeedback::Analyzer::SchemaEntryElement, Q_MOVABLE_TYPE);

#endif // USERFEEDBACK_ANALYZER_SCHEMAENTRYELEMENT_H
