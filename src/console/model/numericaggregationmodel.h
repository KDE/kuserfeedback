/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#ifndef KUSERFEEDBACK_CONSOLE_NUMERICAGGREGATIONMODEL_H
#define KUSERFEEDBACK_CONSOLE_NUMERICAGGREGATIONMODEL_H

#include <core/aggregationelement.h>

#include <QAbstractTableModel>

namespace KUserFeedback {
namespace Console {

class Sample;

class NumericAggregationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit NumericAggregationModel(QObject *parent = nullptr);
    ~NumericAggregationModel();

    void setSourceModel(QAbstractItemModel *model);
    void setAggregation(const AggregationElement &aggr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    void recompute();
    QVector<double> sampleValues(const Sample &s) const;

    QAbstractItemModel *m_sourceModel = nullptr;
    AggregationElement m_aggr;
    struct Data {
        double lowerExtreme = 0.0;
        double lowerQuartile = 0.0;
        double median = 0.0;
        double upperQuartile = 0.0;
        double upperExtreme = 0.0;
    };
    QVector<Data> m_data;

    double m_maxValue = 0.0;
};

}
}

#endif // KUSERFEEDBACK_CONSOLE_NUMERICAGGREGATIONMODEL_H
