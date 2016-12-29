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

#ifndef USERFEEDBACK_ANALYZER_AGGREGATION_H
#define USERFEEDBACK_ANALYZER_AGGREGATION_H

#include <core/aggregationelement.h>

#include <QTypeInfo>

namespace UserFeedback {
namespace Analyzer {

class Aggregation
{
    Q_GADGET
public:
    enum Type {
        None,
        Category,
        RatioSet,
        Numeric,
        XY
    };
    Q_ENUM(Type)

    Aggregation();
    ~Aggregation();

    Type type() const;
    void setType(Type t);

    QVector<AggregationElement> elements() const;
    void setElements(const QVector<AggregationElement> &elements);

private:
    Type m_type = None;
    QVector<AggregationElement> m_elements;
};

}
}

Q_DECLARE_TYPEINFO(UserFeedback::Analyzer::Aggregation, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(UserFeedback::Analyzer::Aggregation::Type)

#endif // USERFEEDBACK_ANALYZER_AGGREGATION_H
