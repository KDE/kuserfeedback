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

#include "productschemaentry.h"
#include <QSharedData>

using namespace UserFeedback::Analyzer;

namespace UserFeedback {
namespace Analyzer {

class ProductSchemaEntryData : public QSharedData
{
public:
    QString name;
    int internalId;
    ProductSchemaEntry::Type type = ProductSchemaEntry::StringType;
};

}
}

ProductSchemaEntry::ProductSchemaEntry() : d(new ProductSchemaEntryData) {}
ProductSchemaEntry::ProductSchemaEntry(const ProductSchemaEntry&) = default;
ProductSchemaEntry::~ProductSchemaEntry() = default;
ProductSchemaEntry& ProductSchemaEntry::operator=(const ProductSchemaEntry&) = default;

int ProductSchemaEntry::internalId() const
{
    return d->internalId;
}

void ProductSchemaEntry::setInternalId(int internalId)
{
    d->internalId = internalId;
}

QString ProductSchemaEntry::name() const
{
    return d->name;
}

void ProductSchemaEntry::setName(const QString& name)
{
    d->name = name;
}

ProductSchemaEntry::Type ProductSchemaEntry::type() const
{
    return d->type;
}

void ProductSchemaEntry::setType(ProductSchemaEntry::Type type)
{
    d->type = type;
}
