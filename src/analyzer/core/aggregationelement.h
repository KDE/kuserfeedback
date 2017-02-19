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

#ifndef USERFEEDBACK_ANALYZER_AGGREGATIONELEMENT_H
#define USERFEEDBACK_ANALYZER_AGGREGATIONELEMENT_H

#include "schemaentry.h"
#include "schemaentryelement.h"

class QJsonArray;
class QJsonObject;

namespace UserFeedback {
namespace Analyzer {

class Product;

class AggregationElement
{
public:
    AggregationElement();
    ~AggregationElement();

    bool operator==(const AggregationElement &other) const;

    enum Type {
        Value,
        Size
    };
    Type type() const;
    void setType(Type t);

    SchemaEntry schemaEntry() const;
    void setSchemaEntry(const SchemaEntry &entry);

    SchemaEntryElement schemaEntryElement() const;
    void setSchemaEntryElement(const SchemaEntryElement &element);

    QString displayString() const;

    QJsonObject toJsonObject() const;
    static QVector<AggregationElement> fromJson(const Product &product, const QJsonArray &a);

private:
    SchemaEntry m_entry;
    SchemaEntryElement m_element;
    Type m_type = Value;
};

}
}

Q_DECLARE_TYPEINFO(UserFeedback::Analyzer::AggregationElement, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(UserFeedback::Analyzer::AggregationElement)

#endif // USERFEEDBACK_ANALYZER_AGGREGATIONELEMENT_H
