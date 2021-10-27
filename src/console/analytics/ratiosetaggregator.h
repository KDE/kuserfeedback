/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_CONSOLE_RATIOSETAGGREGATOR_H
#define KUSERFEEDBACK_CONSOLE_RATIOSETAGGREGATOR_H

#include "aggregator.h"

#include <memory>

namespace QtCharts {
class QPieSeries;
}

namespace KUserFeedback {
namespace Console {

class RatioSetAggregationModel;

class RatioSetAggregator : public Aggregator
{
public:
    RatioSetAggregator();
    ~RatioSetAggregator() override;

    ChartModes chartModes() const override;
    QAbstractItemModel* timeAggregationModel() override;
    QtCharts::QChart* timelineChart() override;
    QtCharts::QChart* singlularChart() override;

private:
    void updateTimelineChart();
    void updateSingularChart();

    void decoratePieSeries(QtCharts::QPieSeries *series) const;

    std::unique_ptr<RatioSetAggregationModel> m_model;
    std::unique_ptr<QtCharts::QChart> m_timelineChart;
    std::unique_ptr<QtCharts::QChart> m_singularChart;
};

}}

#endif // KUSERFEEDBACK_CONSOLE_RATIOSETAGGREGATOR_H
