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

#ifndef USERFEEDBACK_CONSOLE_MAINWINDOW_H
#define USERFEEDBACK_CONSOLE_MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include <memory>

class QAbstractItemModel;
class QNetworkAccessManager;

namespace UserFeedback {

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

    UserFeedback::Provider *m_feedbackProvider;
};
}
}

#endif // USERFEEDBACK_CONSOLE_MAINWINDOW_H
