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
    QVector<ProductSchemaEntry> schema;
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

QVector<ProductSchemaEntry> Product::schema() const
{
    return d->schema;
}

void Product::setSchema(const QVector<ProductSchemaEntry> &schema)
{
    d->schema = schema;
}

QByteArray Product::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("name"), name());
    QJsonDocument doc(obj);
    return doc.toJson();
}

QVector<Product> Product::fromJson(const QByteArray &data)
{
    QVector<Product> products;
    const auto doc = QJsonDocument::fromJson(data);
    foreach (const auto &value, doc.array()) {
        const auto obj = value.toObject();
        Product product;
        product.setName(obj.value(QStringLiteral("name")).toString());
        products.push_back(product);
    }
    return products;
}
