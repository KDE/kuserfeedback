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

#include "schemaentryelement.h"
#include "util.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QSharedData>
#include <QVector>

using namespace KUserFeedback::Console;

namespace KUserFeedback {
namespace Console {
class SchemaEntryElementData : public QSharedData
{
public:
    QString name;
    SchemaEntryElement::Type type = SchemaEntryElement::String;
};

static const struct {
    SchemaEntryElement::Type type;
    const char *name;
} element_type_table[] {
    { SchemaEntryElement::Integer, "int" },
    { SchemaEntryElement::Number, "number" },
    { SchemaEntryElement::String, "string" },
    { SchemaEntryElement::Boolean, "bool" }
};

}
}

SchemaEntryElement::SchemaEntryElement() :
    d(new SchemaEntryElementData)
{
}

SchemaEntryElement::SchemaEntryElement(const SchemaEntryElement&) = default;
SchemaEntryElement::~SchemaEntryElement() = default;
SchemaEntryElement& SchemaEntryElement::operator=(const SchemaEntryElement&) = default;

bool SchemaEntryElement::operator==(const SchemaEntryElement& other) const
{
    return d->name == other.d->name
        && d->type == other.d->type;
}

QString SchemaEntryElement::name() const
{
    return d->name;
}

void SchemaEntryElement::setName(const QString& name)
{
    d->name = name;
}

SchemaEntryElement::Type SchemaEntryElement::type() const
{
    return d->type;
}

void SchemaEntryElement::setType(SchemaEntryElement::Type type)
{
    d->type = type;
}

QJsonObject SchemaEntryElement::toJsonObject() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("name"), d->name);
    obj.insert(QStringLiteral("type"), QLatin1String(element_type_table[d->type].name));
    return obj;
}

QVector<SchemaEntryElement> SchemaEntryElement::fromJson(const QJsonArray& array)
{
    QVector<SchemaEntryElement> res;
    res.reserve(array.size());

    foreach (const auto &v, array) {
        const auto obj = v.toObject();
        SchemaEntryElement e;
        e.setName(obj.value(QLatin1String("name")).toString());
        e.setType(Util::stringToEnum<SchemaEntryElement::Type>(obj.value(QLatin1String("type")).toString(), element_type_table));
        res.push_back(e);
    }

    return res;
}
