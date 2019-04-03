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

#include "product.h"
#include "aggregation.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedData>
#include <QString>

using namespace KUserFeedback::Console;

namespace KUserFeedback {
namespace Console {
class ProductData : public QSharedData
{
public:
    QString name;
    QVector<SchemaEntry> schema;
    QVector<Aggregation> aggregations;
};

}
}

Product::Product() : d(new ProductData) {}
Product::Product(const Product&) = default;
Product::~Product() = default;
Product& Product::operator=(const Product&) = default;

bool Product::isValid() const
{
    return !d->name.isEmpty();
}

QString Product::name() const
{
    return d->name;
}

void Product::setName(const QString& name)
{
    d->name = name;
}

QVector<SchemaEntry> Product::schema() const
{
    return d->schema;
}

SchemaEntry Product::schemaEntry(const QString& name) const
{
    const auto it = std::find_if(d->schema.cbegin(), d->schema.cend(), [name](const auto &entry) {
        return entry.name() == name;
    });
    if (it == d->schema.cend())
        return {};
    return *it;
}

void Product::setSchema(const QVector<SchemaEntry> &schema)
{
    d->schema = schema;
}

QVector<Aggregation> Product::aggregations() const
{
    return d->aggregations;
}

void Product::setAggregations(const QVector<Aggregation>& aggregations)
{
    d->aggregations = aggregations;
}

void Product::addTemplate(const Product& tpl)
{
    // TODO could be done slighty more clever
    d->schema += tpl.schema();
    d->aggregations += tpl.aggregations();
}

QByteArray Product::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("name"), name());
    {
        QJsonArray schema;
        foreach (const auto &s, d->schema)
            schema.push_back(s.toJsonObject());
        obj.insert(QStringLiteral("schema"), schema);
    }

    {
        QJsonArray aggrs;
        foreach (const auto &a, d->aggregations)
            aggrs.push_back(a.toJsonObject());
        obj.insert(QStringLiteral("aggregation"), aggrs);
    }
    QJsonDocument doc(obj);
    return doc.toJson();
}

static Product productFromJsonObject(const QJsonObject &obj)
{
    Product product;
    product.setName(obj.value(QStringLiteral("name")).toString());
    product.setSchema(SchemaEntry::fromJson(obj.value(QStringLiteral("schema")).toArray()));
    product.setAggregations(Aggregation::fromJson(product, obj.value(QLatin1String("aggregation")).toArray()));
    return product;
}

QVector<Product> Product::fromJson(const QByteArray &data)
{
    QVector<Product> products;
    QJsonParseError error;
    const auto doc = QJsonDocument::fromJson(data, &error);
    if (doc.isArray()) {
        const auto array = doc.array();
        products.reserve(array.size());
        foreach (const auto &value, array) {
            const auto obj = value.toObject();
            products.push_back(productFromJsonObject(obj));
        }
    } else if (doc.isObject()) {
        products.push_back(productFromJsonObject(doc.object()));
    } else {
        qDebug() << "Failed to parse product JSON:" << error.errorString() << error.offset;
    }
    return products;
}
