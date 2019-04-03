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

#ifndef KUSERFEEDBACK_CONSOLE_SCHEMAEDITOR_H
#define KUSERFEEDBACK_CONSOLE_SCHEMAEDITOR_H

#include <QWidget>

#include <memory>

class QAction;

namespace KUserFeedback {
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

#endif // KUSERFEEDBACK_CONSOLE_SCHEMAEDITOR_H
