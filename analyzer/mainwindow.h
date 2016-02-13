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

#ifndef USERFEEDBACK_ANALYZER_MAINWINDOW_H
#define USERFEEDBACK_ANALYZER_MAINWINDOW_H

#include <QMainWindow>

#include <memory>

class QNetworkAccessManager;

namespace UserFeedback {

class Provider;

namespace Analyzer {

namespace Ui
{
class MainWindow;
}

class AggregatedDataModel;
class CategoryAggregationModel;
class Chart;
class DataModel;
class Product;
class ProductModel;
class ServerInfo;
class RESTClient;
class SurveyModel;
class TimeAggregationModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

private:
    void connectToServer(const ServerInfo &info);

    void createProduct();
    void deleteProduct();

    void createSurvey();
    void deleteSurvey();

    void logMessage(const QString &msg);
    void logError(const QString &msg);

    Product selectedProduct() const;

    std::unique_ptr<Ui::MainWindow> ui;
    RESTClient *m_restClient;
    ProductModel *m_productModel;
    DataModel *m_dataModel;
    TimeAggregationModel *m_timeAggregationModel;
    CategoryAggregationModel *m_versionModel;
    AggregatedDataModel *m_aggregatedDataModel;
    SurveyModel *m_surveyModel;
    Chart *m_chart;

    UserFeedback::Provider *m_feedbackProvider;
};
}
}

#endif // USERFEEDBACK_ANALYZER_MAINWINDOW_H
