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

#ifndef USERFEEDBACK_ANALYZER_TIMEAGGREGATIONMODEL_H
#define USERFEEDBACK_ANALYZER_TIMEAGGREGATIONMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QVector>

namespace UserFeedback {
namespace Analyzer {

class Sample;

class TimeAggregationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum AggregationMode {
        AggregateYear,
        AggregateMonth,
        AggregateWeek,
        AggregateDay
    };

    enum Role {
        DateTimeRole = Qt::UserRole + 1,
        MaximumValueRole,
        TimeDisplayRole,
        DataDisplayRole,
        SamplesRole,
        AllSamplesRole
    };

    explicit TimeAggregationModel(QObject *parent = nullptr);
    ~TimeAggregationModel();

    void setSourceModel(QAbstractItemModel *model);

    AggregationMode aggregationMode() const;
    void setAggregationMode(AggregationMode mode);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    void reload();
    QDateTime aggregate(const QDateTime &dt) const;
    QString timeToString(const QDateTime &dt) const;

    QAbstractItemModel *m_sourceModel = nullptr;
    struct Data {
        QDateTime time;
        QVector<Sample> samples;
    };
    QVector<Data> m_data;
    int m_maxValue = 0;
    AggregationMode m_mode = AggregateYear;
};

}
}

#endif // USERFEEDBACK_ANALYZER_TIMEAGGREGATIONMODEL_H
