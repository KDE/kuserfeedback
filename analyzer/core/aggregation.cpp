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

using namespace UserFeedback::Analyzer;

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
