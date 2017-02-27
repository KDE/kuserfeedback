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

#ifndef USERFEEDBACK_CONSOLE_SURVEYEDITOR_H
#define USERFEEDBACK_CONSOLE_SURVEYEDITOR_H

#include <core/product.h>

#include <QWidget>

#include <memory>

namespace UserFeedback {
namespace Console {

class RESTClient;
class SurveyModel;

namespace Ui
{
class SurveyEditor;
}

class SurveyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SurveyEditor(QWidget *parent = nullptr);
    ~SurveyEditor();

    void setRESTClient(RESTClient *client);
    void setProduct(const Product &product);

signals:
    void logMessage(const QString &msg);

private:
    void createSurvey();
    void deleteSurvey();
    void updateActions();

private:
    std::unique_ptr<Ui::SurveyEditor> ui;
    SurveyModel *m_surveyModel;
    RESTClient *m_restClient = nullptr;
    Product m_product;
};

}
}

#endif // USERFEEDBACK_CONSOLE_SURVEYEDITOR_H
