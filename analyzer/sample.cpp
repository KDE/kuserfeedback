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

#include "sample.h"
#include "product.h"
#include "productschemaentry.h"
#include "ratioset.h"

#include <QDateTime>
#include <QDebug>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedData>
#include <QVariant>

using namespace UserFeedback::Analyzer;

class UserFeedback::Analyzer::SampleData : public QSharedData
{
public:
    QDateTime timestamp;
    QHash<QString, QVariant> data;
};

Sample::Sample() : d(new SampleData) {}
Sample::Sample(const Sample&) = default;
Sample::~Sample() = default;
Sample& Sample::operator=(const Sample&) = default;

QDateTime Sample::timestamp() const
{
    return d->timestamp;
}

QVariant Sample::value(const QString &name) const
{
    return d->data.value(name);
}

QVector<Sample> Sample::fromJson(const QByteArray &json, const Product &product)
{
    const auto array = QJsonDocument::fromJson(json).array();
    QVector<Sample> samples;
    samples.reserve(array.size());
    for (auto it = array.begin(); it != array.end(); ++it) {
        const auto obj = it->toObject();
        Sample s;
        s.d->timestamp = QDateTime::fromString(obj.value(QStringLiteral("timestamp")).toString(), Qt::ISODate);
        foreach (const auto &entry, product.schema()) {
            if (!obj.contains(entry.name()))
                continue;
            switch (entry.type()) {
                case ProductSchemaEntry::InvalidType:
                    break;
                case ProductSchemaEntry::StringType:
                    s.d->data.insert(entry.name(), obj.value(entry.name()).toString());
                    break;
                case ProductSchemaEntry::IntegerType:
                    s.d->data.insert(entry.name(), obj.value(entry.name()).toInt());
                    break;
                case ProductSchemaEntry::StringListType:
                {
                    QStringList l;
                    const auto a = obj.value(entry.name()).toArray();
                    l.reserve(a.size());
                    foreach (const auto &v, a)
                        l.push_back(v.toString());
                    s.d->data.insert(entry.name(), l);
                    break;
                }
                case ProductSchemaEntry::RatioSetType:
                {
                    const auto set = RatioSet::fromJson(obj.value(entry.name()).toObject());
                    s.d->data.insert(entry.name(), QVariant::fromValue(set));
                    break;
                }
            }
        }
        samples.push_back(s);
    }
    return samples;
}