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

#ifndef USERFEEDBACK_CONSOLE_AGGREGATOR_H
#define USERFEEDBACK_CONSOLE_AGGREGATOR_H

#include <core/aggregation.h>

#include <memory>

class QAbstractItemModel;

namespace QtCharts {
class QChart;
}

namespace UserFeedback {
namespace Console {

class Aggregation;
class SingleRowFilterProxyModel;

class Aggregator
{
public:
    explicit Aggregator();
    virtual ~Aggregator();

    enum ChartMode {
        None = 0x0,
        Singular = 0x1,
        Timeline = 0x2
    };
    Q_DECLARE_FLAGS(ChartModes, ChartMode)

    Aggregation aggregation() const;
    void setAggregation(const Aggregation &aggr);

    QAbstractItemModel *sourceModel() const;
    void setSourceModel(QAbstractItemModel *model);

    virtual QString displayName() const;

    virtual ChartModes chartModes() const;

    virtual QAbstractItemModel *timeAggregationModel();
    QAbstractItemModel* singularAggregationModel();
    virtual void setSingularTime(int row);

    virtual QtCharts::QChart *singlularChart();
    virtual QtCharts::QChart *timelineChart();

private:
    QAbstractItemModel *m_sourceModel = nullptr;
    std::unique_ptr<SingleRowFilterProxyModel> m_singularModel;
    Aggregation m_aggregation;
};

}}

Q_DECLARE_METATYPE(UserFeedback::Console::Aggregator*)
Q_DECLARE_OPERATORS_FOR_FLAGS(UserFeedback::Console::Aggregator::ChartModes)

#endif // USERFEEDBACK_CONSOLE_AGGREGATOR_H
