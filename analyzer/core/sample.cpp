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
#include <core/product.h>
#include <core/schemaentry.h>
#include <core/schemaentryelement.h>
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
            switch (entry.dataType()) {
                case SchemaEntry::Scalar:
                {
                    const auto entryData = obj.value(entry.name()).toObject();
                    foreach (const auto &elem, entry.elements()) {
                        if (!entryData.contains(elem.name()))
                            continue;
                        // TODO schema-dependent type conversion
                        s.d->data.insert(entry.name() + QLatin1Char('.') + elem.name(), entryData.value(elem.name()).toVariant());
                    }
                    break;
                }
                case SchemaEntry::List:
                {
                    const auto entryArray = obj.value(entry.name()).toArray();
                    QVariantList l;
                    l.reserve(entryArray.size());
                    foreach (const auto &entryDataValue, entryArray) {
                        const auto entryData = entryDataValue.toObject();
                        QVariantMap m;
                        foreach (const auto &elem, entry.elements()) {
                            if (!entryData.contains(elem.name()))
                                continue;
                            // TODO schema-dependent type conversion
                            m.insert(elem.name(), entryData.value(elem.name()).toVariant());
                        }
                        l.push_back(m);
                    }
                    s.d->data.insert(entry.name(), l);
                    break;
                }
                case SchemaEntry::Map:
                {
                    const auto entryMap = obj.value(entry.name()).toObject();
                    QVariantMap m;
                    for (auto it = entryMap.begin(); it != entryMap.end(); ++it) {
                        const auto entryData = it.value().toObject();
                        QVariantMap m2;
                        foreach (const auto &elem, entry.elements()) {
                            if (!entryData.contains(elem.name()))
                                continue;
                            // TODO schema-dependent type conversion
                            m2.insert(elem.name(), entryData.value(elem.name()).toVariant());
                        }
                        m.insert(it.key(), m2);
                    }
                    s.d->data.insert(entry.name(), m);
                    break;
                }
            }
        }
        samples.push_back(s);
    }
    return samples;
}
