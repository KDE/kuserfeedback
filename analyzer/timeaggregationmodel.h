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
#include <QVector>

namespace UserFeedback {
namespace Analyzer {

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
    explicit TimeAggregationModel(QObject *parent = nullptr);
    ~TimeAggregationModel();

    void setSourceModel(QAbstractItemModel *model);

    void setAggregationMode(AggregationMode mode);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    void reload();
    QString aggregate(const QDateTime &dt) const;

    QAbstractItemModel *m_sourceModel = nullptr;
    struct Data {
        QString timeLabel;
        int samples;
    };
    QVector<Data> m_data;
    AggregationMode m_mode = AggregateYear;
};

}
}

#endif // USERFEEDBACK_ANALYZER_TIMEAGGREGATIONMODEL_H
