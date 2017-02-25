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

#ifndef USERFEEDBACK_CONSOLE_SCHEMAEDITOR_H
#define USERFEEDBACK_CONSOLE_SCHEMAEDITOR_H

#include <QWidget>

#include <memory>

class QAction;

namespace UserFeedback {
namespace Console {

class Product;
class RESTClient;

namespace Ui {
class SchemaEditor;
}

class SchemaEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SchemaEditor(QWidget *parent = nullptr);
    ~SchemaEditor();

    void setRESTClient(RESTClient *client);

    Product product() const;
    void setProduct(const Product &product);

    bool isDirty() const;

signals:
    void productChanged(const Product &product);
    void logMessage(const QString &msg);

private:
    void save();
    void exportSchema();
    void importSchema();

    void setDirty(bool dirty = true);
    void updateState();

    std::unique_ptr<Ui::SchemaEditor> ui;
    RESTClient *m_restClient = nullptr;
    QAction *m_createFromTemplateAction;
    bool m_isDirty = false;
};

}
}

#endif // USERFEEDBACK_CONSOLE_SCHEMAEDITOR_H
