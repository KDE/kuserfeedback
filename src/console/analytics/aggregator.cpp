/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include "aggregator.h"

#include <model/singlerowfilterproxymodel.h>

using namespace UserFeedback::Console;

Aggregator::Aggregator() = default;
Aggregator::~Aggregator() = default;

Aggregation Aggregator::aggregation() const
{
    return m_aggregation;
}

void Aggregator::setAggregation(const Aggregation& aggr)
{
    m_aggregation = aggr;
}

QAbstractItemModel* Aggregator::sourceModel() const
{
    return m_sourceModel;
}

void Aggregator::setSourceModel(QAbstractItemModel* model)
{
    m_sourceModel = model;
}

QString Aggregator::displayName() const
{
    return m_aggregation.name();
}

Aggregator::ChartModes Aggregator::chartModes() const
{
    return None;
}

QAbstractItemModel* Aggregator::timeAggregationModel()
{
    return nullptr;
}

QAbstractItemModel* Aggregator::singularAggregationModel()
{
    if (!m_singularModel) {
        m_singularModel.reset(new SingleRowFilterProxyModel);
        m_singularModel->setSourceModel(timeAggregationModel());
    }
    return m_singularModel.get();
}

void Aggregator::setSingularTime(int row)
{
    singularAggregationModel();
    m_singularModel->setRow(row);
}

QtCharts::QChart *Aggregator::singlularChart()
{
    return nullptr;
}

QtCharts::QChart *Aggregator::timelineChart()
{
    return nullptr;
}
