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

#ifndef KUSERFEEDBACK_CONSOLE_AGGREGATOR_H
#define KUSERFEEDBACK_CONSOLE_AGGREGATOR_H

#include <core/aggregation.h>

#include <memory>

class QAbstractItemModel;

namespace QtCharts {
class QChart;
}

namespace KUserFeedback {
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

Q_DECLARE_METATYPE(KUserFeedback::Console::Aggregator*)
Q_DECLARE_OPERATORS_FOR_FLAGS(KUserFeedback::Console::Aggregator::ChartModes)

#endif // KUSERFEEDBACK_CONSOLE_AGGREGATOR_H
