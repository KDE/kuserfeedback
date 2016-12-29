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


#ifndef USERFEEDBACK_ANALYZER_AGGREGATIONELEMENTMODEL_H
#define USERFEEDBACK_ANALYZER_AGGREGATIONELEMENTMODEL_H

#include <core/aggregation.h>

#include <QAbstractListModel>
#include <QVector>

namespace UserFeedback {
namespace Analyzer {

class Product;

class AggregationElementModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AggregationElementModel(QObject *parent = nullptr);
    ~AggregationElementModel();

    void setProduct(const Product &product);

    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role) const override;
    QModelIndexList match(const QModelIndex & start, int role, const QVariant & value, int hits, Qt::MatchFlags flags) const override;

private:
    QVector<Aggregation::Element> m_elements;
};
}
}

#endif // USERFEEDBACK_ANALYZER_AGGREGATIONELEMENTMODEL_H
