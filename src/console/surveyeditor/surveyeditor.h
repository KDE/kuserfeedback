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

#ifndef KUSERFEEDBACK_CONSOLE_SURVEYEDITOR_H
#define KUSERFEEDBACK_CONSOLE_SURVEYEDITOR_H

#include <core/product.h>

#include <QWidget>

#include <memory>

namespace KUserFeedback {
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
    void editSurvey();
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

#endif // KUSERFEEDBACK_CONSOLE_SURVEYEDITOR_H
