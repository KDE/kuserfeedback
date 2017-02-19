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

#ifndef USERFEEDBACK_CONSOLE_NUMERICAGGREGATIONMODEL_H
#define USERFEEDBACK_CONSOLE_NUMERICAGGREGATIONMODEL_H

#include <QAbstractTableModel>

namespace UserFeedback {
namespace Console {

class NumericAggregationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit NumericAggregationModel(QObject *parent = nullptr);
    ~NumericAggregationModel();

    void setSourceModel(QAbstractItemModel *model);
    void setAggregationValue(const QString &aggrValue);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    void recompute();

    QAbstractItemModel *m_sourceModel = nullptr;
    QString m_aggrValue;
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

#endif // USERFEEDBACK_CONSOLE_NUMERICAGGREGATIONMODEL_H
