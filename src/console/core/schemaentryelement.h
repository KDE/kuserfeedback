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

#ifndef KUSERFEEDBACK_CONSOLE_SCHEMAENTRYELEMENT_H
#define KUSERFEEDBACK_CONSOLE_SCHEMAENTRYELEMENT_H

#include <qobjectdefs.h>
#include <QMetaType>
#include <QSharedDataPointer>

QT_BEGIN_NAMESPACE
class QJsonArray;
class QJsonObject;
template <typename T> class QVector;
QT_END_NAMESPACE

namespace KUserFeedback {
namespace Console {

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

Q_DECLARE_METATYPE(KUserFeedback::Console::SchemaEntryElement)
Q_DECLARE_METATYPE(KUserFeedback::Console::SchemaEntryElement::Type)
Q_DECLARE_TYPEINFO(KUserFeedback::Console::SchemaEntryElement, Q_MOVABLE_TYPE);

#endif // KUSERFEEDBACK_CONSOLE_SCHEMAENTRYELEMENT_H
