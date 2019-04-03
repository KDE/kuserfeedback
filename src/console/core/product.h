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

#ifndef KUSERFEEDBACK_CONSOLE_PRODUCT_H
#define KUSERFEEDBACK_CONSOLE_PRODUCT_H

#include "schemaentry.h"

#include <QMetaType>
#include <QSharedDataPointer>
#include <QVector>

class QString;

namespace KUserFeedback {
namespace Console {

class Aggregation;
class ProductData;

/** Product data. */
class Product
{
public:
    Product();
    Product(const Product&);
    ~Product();
    Product& operator=(const Product&);

    bool isValid() const;

    QString name() const;
    void setName(const QString &name);

    QVector<SchemaEntry> schema() const;
    void setSchema(const QVector<SchemaEntry>& schema);
    SchemaEntry schemaEntry(const QString &name) const;

    QVector<Aggregation> aggregations() const;
    void setAggregations(const QVector<Aggregation> &aggregations);

    void addTemplate(const Product &tpl);

    QByteArray toJson() const;
    static QVector<Product> fromJson(const QByteArray &data);

private:
    QSharedDataPointer<ProductData> d;

};

}
}

Q_DECLARE_TYPEINFO(KUserFeedback::Console::Product, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(KUserFeedback::Console::Product)

#endif // KUSERFEEDBACK_CONSOLE_PRODUCT_H
