/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef KUSERFEEDBACK_CONSOLE_AGGREGATIONELEMENT_H
#define KUSERFEEDBACK_CONSOLE_AGGREGATIONELEMENT_H

#include "schemaentry.h"
#include "schemaentryelement.h"

class QJsonArray;
class QJsonObject;

namespace KUserFeedback {
namespace Console {

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

    bool isValid() const;

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

Q_DECLARE_TYPEINFO(KUserFeedback::Console::AggregationElement, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(KUserFeedback::Console::AggregationElement)

#endif // KUSERFEEDBACK_CONSOLE_AGGREGATIONELEMENT_H
