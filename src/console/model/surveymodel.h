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

#ifndef KUSERFEEDBACK_CONSOLE_SURVEYMODEL_H
#define KUSERFEEDBACK_CONSOLE_SURVEYMODEL_H

#include <core/product.h>
#include <core/survey.h>

#include <QAbstractTableModel>

namespace KUserFeedback {
namespace Console {

class RESTClient;

class SurveyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Role {
        SurveyRole = Qt::UserRole + 1
    };

    explicit SurveyModel(QObject *parent = nullptr);
    ~SurveyModel();

    void setRESTClient(RESTClient *client);
    void setProduct(const Product &product);
    void reload();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    RESTClient *m_restClient = nullptr;
    Product m_product;
    QVector<Survey> m_surveys;
};

}
}

#endif // KUSERFEEDBACK_CONSOLE_SURVEYMODEL_H
