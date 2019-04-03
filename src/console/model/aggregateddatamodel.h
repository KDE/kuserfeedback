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
