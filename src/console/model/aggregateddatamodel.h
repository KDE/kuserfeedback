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

#ifndef KUSERFEEDBACK_CONSOLE_AGGREGATEDDATAMODEL_H
#define KUSERFEEDBACK_CONSOLE_AGGREGATEDDATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>

namespace KUserFeedback {
namespace Console {

/** Joint model for all time-aggregated data, for user display and export. */
class AggregatedDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AggregatedDataModel(QObject *parent = nullptr);
    ~AggregatedDataModel();

    void addSourceModel(QAbstractItemModel *model, const QString &prefix = QString());
    void clear();

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    void recreateColumnMapping();

    QVector<QAbstractItemModel*> m_models;
    QVector<QString> m_prefixes;
    QVector<int> m_columnMapping;
    QVector<int> m_columnOffset;
};

}
}

#endif // KUSERFEEDBACK_CONSOLE_AGGREGATEDDATAMODEL_H
