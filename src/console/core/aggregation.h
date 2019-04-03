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

#ifndef KUSERFEEDBACK_CONSOLE_AGGREGATION_H
#define KUSERFEEDBACK_CONSOLE_AGGREGATION_H

#include <core/aggregationelement.h>

#include <QTypeInfo>

class QJsonArray;
class QJsonObject;

namespace KUserFeedback {
namespace Console {

class Product;

class Aggregation
{
    Q_GADGET
public:
    enum Type {
        None,
        Category,
        RatioSet,
        Numeric
    };
    Q_ENUM(Type)

    Aggregation();
    ~Aggregation();

    bool isValid() const;

    Type type() const;
    void setType(Type t);

    QString name() const;
    void setName(const QString &name);

    QVector<AggregationElement> elements() const;
    void setElements(const QVector<AggregationElement> &elements);

    QJsonObject toJsonObject() const;
    static QVector<Aggregation> fromJson(const Product &product, const QJsonArray &a);

private:
    Type m_type = None;
    QString m_name;
    QVector<AggregationElement> m_elements;
};

}
}

Q_DECLARE_TYPEINFO(KUserFeedback::Console::Aggregation, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(KUserFeedback::Console::Aggregation::Type)

#endif // KUSERFEEDBACK_CONSOLE_AGGREGATION_H
