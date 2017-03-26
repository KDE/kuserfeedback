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

#ifndef USERFEEDBACK_CONSOLE_NUMERICAGGREGATOR_H
#define USERFEEDBACK_CONSOLE_NUMERICAGGREGATOR_H

#include "aggregator.h"

#include <memory>

namespace UserFeedback {
namespace Console {

class NumericAggregationModel;

class NumericAggregator :  public Aggregator
{
public:
    NumericAggregator();
    ~NumericAggregator();

    ChartModes chartModes() const override;
    QAbstractItemModel* timeAggregationModel() override;
    QtCharts::QChart* timelineChart() override;

private:
    void updateTimelineChart();

    std::unique_ptr<NumericAggregationModel> m_model;
    std::unique_ptr<QtCharts::QChart> m_timelineChart;
};

}}

#endif // USERFEEDBACK_CONSOLE_NUMERICAGGREGATOR_H
