/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include "aggregator.h"

#include <model/singlerowfilterproxymodel.h>

using namespace KUserFeedback::Console;

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
