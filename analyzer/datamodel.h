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

#ifndef USERFEEDBACK_ANALYZER_DATAMODEL_H
#define USERFEEDBACK_ANALYZER_DATAMODEL_H

#include <QAbstractTableModel>
#include <QJsonArray>

namespace UserFeedback {
namespace Analyzer {

class RESTClient;
class Sample;

/** Raw data model showing reported data of a product. */
class DataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum {
        SampleRole = Qt::UserRole + 1
    };

    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    void setRESTClient(RESTClient *client);
    void setProductId(const QString &productId);
    void reload();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QString m_productId;
    RESTClient *m_restClient = nullptr;
    QVector<Sample> m_data;
};

}
}

#endif // USERFEEDBACK_ANALYZER_DATAMODEL_H
