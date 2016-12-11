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

#ifndef USERFEEDBACK_ANALYZER_SCHEMAEDITWIDGET_H
#define USERFEEDBACK_ANALYZER_SCHEMAEDITWIDGET_H

#include <QWidget>

#include <memory>

namespace UserFeedback {
namespace Analyzer {

namespace Ui
{
class SchemaEditWidget;
}

class Product;
class RESTClient;
class SchemaModel;

class SchemaEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SchemaEditWidget(QWidget *parent = nullptr);
    ~SchemaEditWidget();

    void setRESTClient(RESTClient *client);
    void setProduct(const Product &product);

signals:
    void productChanged(const Product &product);
    void logMessage(const QString &msg);

private:
    void addEntry();
    void deleteEntry();
    void save();
    void exportSchema();
    void importSchema();

    void updateState();

    std::unique_ptr<Ui::SchemaEditWidget> ui;
    RESTClient *m_restClient;
    SchemaModel *m_schemaModel;
    QAction *m_createFromTemplateAction;
};

}
}

#endif // USERFEEDBACK_ANALYZER_SCHEMAEDITWIDGET_H
