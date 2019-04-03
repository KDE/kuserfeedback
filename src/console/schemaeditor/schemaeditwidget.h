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

#ifndef KUSERFEEDBACK_CONSOLE_SCHEMAEDITWIDGET_H
#define KUSERFEEDBACK_CONSOLE_SCHEMAEDITWIDGET_H

#include <QWidget>

#include <memory>

namespace KUserFeedback {
namespace Console {

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
    Product product() const;
    void setProduct(const Product &product);

signals:
    void productChanged();
    void logMessage(const QString &msg);

private:
    void addSource();
    void addSourceEntry();
    void deleteEntry();
    void contextMenu(QPoint pos);

    void updateState();

    QModelIndex currentSource() const;

    std::unique_ptr<Ui::SchemaEditWidget> ui;
    RESTClient *m_restClient;
    SchemaModel *m_schemaModel;
};

}
}

#endif // KUSERFEEDBACK_CONSOLE_SCHEMAEDITWIDGET_H
