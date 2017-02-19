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

#include "aggregation.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace UserFeedback::Analyzer;

static const struct {
    Aggregation::Type type;
    const char *name;
} aggregation_types_table[] {
    { Aggregation::None, "none" },
    { Aggregation::Category, "category" },
    { Aggregation::RatioSet, "ratio_set" },
    { Aggregation::Numeric, "numeric" },
    { Aggregation::XY, "xy" }
};

Aggregation::Aggregation() = default;
Aggregation::~Aggregation() = default;

Aggregation::Type Aggregation::type() const
{
    return m_type;
}

void Aggregation::setType(Aggregation::Type t)
{
    m_type = t;
}

QVector<AggregationElement> Aggregation::elements() const
{
    return m_elements;
}

void Aggregation::setElements(const QVector<AggregationElement>& elements)
{
    m_elements = elements;
}

QJsonObject Aggregation::toJsonObject() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("type"), QLatin1String(aggregation_types_table[m_type].name));
    QJsonArray elems;
    for (const auto &e : m_elements)
        elems.push_back(e.toJsonObject());
    obj.insert(QStringLiteral("elements"), elems);
    return obj;
}

QVector<Aggregation> Aggregation::fromJson(const Product &product, const QJsonArray& a)
{
    QVector<Aggregation> aggrs;
    aggrs.reserve(a.size());
    for (const auto &v : a) {
        if (!v.isObject())
            continue;
        const auto obj = v.toObject();

        Aggregation aggr;
        aggr.setType(Util::stringToEnum<Aggregation::Type>(obj.value(QLatin1String("type")).toString(), aggregation_types_table));
        aggr.setElements(AggregationElement::fromJson(product, obj.value(QLatin1String("elements")).toArray()));
        aggrs.push_back(aggr);
    }
    return aggrs;
}
