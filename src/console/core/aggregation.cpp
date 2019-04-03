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

#include "aggregation.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace KUserFeedback::Console;

static const struct {
    Aggregation::Type type;
    const char *name;
} aggregation_types_table[] {
    { Aggregation::None, "none" },
    { Aggregation::Category, "category" },
    { Aggregation::RatioSet, "ratio_set" },
    { Aggregation::Numeric, "numeric" }
};

Aggregation::Aggregation() = default;
Aggregation::~Aggregation() = default;

bool Aggregation::isValid() const
{
    return m_type != None && m_elements.size() > 0;
}

Aggregation::Type Aggregation::type() const
{
    return m_type;
}

void Aggregation::setType(Aggregation::Type t)
{
    m_type = t;
}

QString Aggregation::name() const
{
    return m_name;
}

void Aggregation::setName(const QString& name)
{
    m_name = name;
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
    obj.insert(QStringLiteral("name"), m_name);
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
        aggr.setName(obj.value(QLatin1String("name")).toString());
        aggr.setElements(AggregationElement::fromJson(product, obj.value(QLatin1String("elements")).toArray()));
        aggrs.push_back(aggr);
    }
    return aggrs;
}
