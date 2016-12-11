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

void Product::setSchema(const QVector<SchemaEntry> &schema)
{
    d->schema = schema;
}

QByteArray Product::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("name"), name());
    QJsonArray schema;
    foreach (const auto &s, d->schema)
        schema.push_back(s.toJsonObject());
    obj.insert(QStringLiteral("schema"), schema);
    QJsonDocument doc(obj);
    return doc.toJson();
}

static Product productFromJsonObject(const QJsonObject &obj)
{
    Product product;
    product.setName(obj.value(QStringLiteral("name")).toString());
    product.setSchema(SchemaEntry::fromJson(obj.value(QStringLiteral("schema")).toArray()));
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
