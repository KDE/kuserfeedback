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

#include "ratiosetaggregator.h"

#include <model/ratiosetaggregationmodel.h>

using namespace UserFeedback::Analyzer;

RatioSetAggregator::RatioSetAggregator() = default;
RatioSetAggregator::~RatioSetAggregator() = default;

Aggregator::ChartModes RatioSetAggregator::chartModes() const
{
    Aggregator::ChartModes modes = None;
    if (aggregation().elements().size() == 1)
        modes |= Timeline;
    return modes;
}

QString RatioSetAggregator::displayName() const
{
    const auto e = aggregation().elements().at(0);
    return e.schemaEntry().name();
}

QAbstractItemModel* RatioSetAggregator::timeAggregationModel()
{
    if (!m_model && !aggregation().elements().isEmpty()) {
        m_model.reset(new RatioSetAggregationModel);
        m_model->setSourceModel(sourceModel());
        const auto e = aggregation().elements().at(0);
        m_model->setAggregationValue(e.schemaEntry().name());
    }
    return m_model.get();
}

QtCharts::QChart* RatioSetAggregator::timelineChart()
{
    return nullptr;
}
