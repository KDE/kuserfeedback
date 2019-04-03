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

#ifndef KUSERFEEDBACK_CONSOLE_MAINWINDOW_H
#define KUSERFEEDBACK_CONSOLE_MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include <memory>

class QAbstractItemModel;
class QNetworkAccessManager;

namespace KUserFeedback {

class Provider;

namespace Console {

namespace Ui
{
class MainWindow;
}

class Product;
class ProductModel;
class ServerInfo;
class RESTClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void connectToServer(const ServerInfo &info);

    void createProduct();
    void deleteProduct();
    void productSelected();
    void importProduct();
    void exportProduct();

    void logMessage(const QString &msg);
    void logError(const QString &msg);

    Product selectedProduct() const;

    void updateActions();

    template <typename T> void addView(T *view, QMenu *menu);

    std::unique_ptr<Ui::MainWindow> ui;
    RESTClient *m_restClient;
    ProductModel *m_productModel;

    Provider *m_feedbackProvider;
};
}
}

#endif // KUSERFEEDBACK_CONSOLE_MAINWINDOW_H
