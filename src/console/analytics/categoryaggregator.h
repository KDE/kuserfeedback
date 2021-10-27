/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_CONSOLE_CATEGORYAGGREGATOR_H
#define KUSERFEEDBACK_CONSOLE_CATEGORYAGGREGATOR_H

#include "aggregator.h"

#include <memory>
#include <vector>

namespace QtCharts {
class QPieSeries;
}

namespace KUserFeedback {
namespace Console {

class CategoryAggregationModel;
class SingleRowFilterProxyModel;

class CategoryAggregator : public Aggregator
{
public:
    CategoryAggregator();
    ~CategoryAggregator() override;

    ChartModes chartModes() const override;
    QAbstractItemModel* timeAggregationModel() override;
    QtCharts::QChart* timelineChart() override;
    QtCharts::QChart* singlularChart() override;
    void setSingularTime(int row) override;

private:
    void updateTimelineChart();
    void updateSingularChart();
    void decorateSeries(QtCharts::QPieSeries *series, int ring) const;

    std::unique_ptr<CategoryAggregationModel> m_model;
    std::unique_ptr<QtCharts::QChart> m_timelineChart;
    std::unique_ptr<QtCharts::QChart> m_singularChart;
    std::vector<SingleRowFilterProxyModel*> m_hierachicalCategories;
};

}}

#endif // KUSERFEEDBACK_CONSOLE_CATEGORYAGGREGATOR_H
