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

#ifndef USERFEEDBACK_ANALYZER_CHART_H
#define USERFEEDBACK_ANALYZER_CHART_H

#include <QObject>

#include <memory>

class QAbstractItemModel;

namespace QtCharts {
class QChart;
class QDateTimeAxis;
class QValueAxis;
}

namespace UserFeedback {
namespace Analyzer {

class Chart : public QObject
{
    Q_OBJECT
public:
    explicit Chart(QObject *parent = nullptr);
    ~Chart();

    QtCharts::QChart *chart() const;

    void setModel(QAbstractItemModel* model);

private:
    void modelReset();

    std::unique_ptr<QtCharts::QChart> m_chart;
    QAbstractItemModel *m_model = nullptr;
    QtCharts::QDateTimeAxis *m_xAxis;
    QtCharts::QValueAxis *m_yAxis;
};

}
}

#endif // USERFEEDBACK_ANALYZER_CHART_H
