/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_CONSOLE_ANALYTICSVIEW_H
#define KUSERFEEDBACK_CONSOLE_ANALYTICSVIEW_H

#include <core/product.h>

#include <QVector>
#include <QWidget>

#include <memory>

class QAbstractItemModel;

namespace QtCharts {
class QChart;
}

namespace KUserFeedback {
namespace Console {

class Aggregator;
class AggregatedDataModel;
class DataModel;
class RESTClient;
class TimeAggregationModel;

namespace Ui
{
class AnalyticsView;
}

class AnalyticsView : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyticsView(QWidget *parent = nullptr);
    ~AnalyticsView();

    void setProduct(const Product &product);
    void setRESTClient(RESTClient *client);

signals:
    void logMessage(const QString &msg);

private:
    void chartSelected();
    void updateChart();
    void updateTimeSliderRange();

    Aggregator* createAggregator(const Aggregation &aggr) const;

    void exportData();
    void importData();
    void exportChart();

    std::unique_ptr<Ui::AnalyticsView> ui;

    RESTClient *m_client = nullptr;
    DataModel *m_dataModel;
    TimeAggregationModel *m_timeAggregationModel;
    AggregatedDataModel *m_aggregatedDataModel;

    QVector<Aggregator*> m_aggregators;

    std::unique_ptr<QtCharts::QChart> m_nullSingularChart;
    std::unique_ptr<QtCharts::QChart> m_nullTimelineChart;
};
}
}

#endif // KUSERFEEDBACK_CONSOLE_ANALYTICSVIEW_H
