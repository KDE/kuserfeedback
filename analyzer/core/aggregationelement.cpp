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

#include "aggregationelement.h"

using namespace UserFeedback::Analyzer;

AggregationElement::AggregationElement() = default;
AggregationElement::~AggregationElement() = default;

SchemaEntry AggregationElement::schemaEntry() const
{
    return m_entry;
}

void AggregationElement::setSchemaEntry(const SchemaEntry& entry)
{
    m_entry = entry;
}

SchemaEntryElement AggregationElement::schemaEntryElement() const
{
    return m_element;
}

void AggregationElement::setSchemaEntryElement(const SchemaEntryElement& element)
{
    m_element = element;
}

AggregationElement::Type AggregationElement::type() const
{
    return m_type;
}

void AggregationElement::setType(AggregationElement::Type t)
{
    m_type = t;
}

QString AggregationElement::displayString() const
{
    switch (m_type) {
        case Value:
           return m_entry.name() + QLatin1Char('.') + m_element.name();
        case Size:
            return m_entry.name() + QLatin1String("[size]");
    }
    Q_UNREACHABLE();
}

bool AggregationElement::operator==(const AggregationElement &other) const
{
    if (m_type != other.m_type)
        return false;

    switch (m_type) {
        case Value:
            return m_element.name() == other.m_element.name() && m_entry.name() == other.m_entry.name();
        case Size:
            return m_element.name() == other.m_element.name();
    }
    Q_UNREACHABLE();
}
