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

#include "product.h"
#include "aggregation.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedData>
#include <QString>

using namespace UserFeedback::Analyzer;

namespace UserFeedback {
namespace Analyzer {
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
    const auto doc = QJsonDocument::fromJson(data);
    if (doc.isArray()) {
        const auto array = doc.array();
        products.reserve(array.size());
        foreach (const auto &value, array) {
            const auto obj = value.toObject();
            products.push_back(productFromJsonObject(obj));
        }
    } else if (doc.isObject()) {
        products.push_back(productFromJsonObject(doc.object()));
    }
    return products;
}
