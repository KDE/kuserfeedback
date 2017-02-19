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

#include "surveyeditor.h"
#include "ui_surveyeditor.h"

#include "surveydialog.h"

#include <model/surveymodel.h>
#include <rest/restapi.h>

#include <QNetworkReply>

using namespace UserFeedback::Analyzer;

SurveyEditor::SurveyEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SurveyEditor),
    m_surveyModel(new SurveyModel(this))
{
    ui->setupUi(this);
    ui->surveyView->setModel(m_surveyModel);

    ui->actionAddSurvey->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    connect(ui->actionAddSurvey, &QAction::triggered, this, &SurveyEditor::createSurvey);
    ui->actionDeleteSurvey->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    connect(ui->actionDeleteSurvey, &QAction::triggered, this, &SurveyEditor::deleteSurvey);

    addActions({ ui->actionAddSurvey, ui->actionDeleteSurvey });
}

SurveyEditor::~SurveyEditor() = default;

void SurveyEditor::setRESTClient(RESTClient* client)
{
    m_restClient = client;
    m_surveyModel->setRESTClient(client);
}

void SurveyEditor::setProduct(const Product& product)
{
    m_product = product;
    m_surveyModel->setProduct(product);
}

void SurveyEditor::createSurvey()
{
    if (!m_product.isValid())
        return;
    SurveyDialog dlg(this);
    if (!dlg.exec())
        return;
    auto reply = RESTApi::createSurvey(m_restClient, m_product, dlg.survey());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit logMessage(QString::fromUtf8(reply->readAll()));
        }
        m_surveyModel->reload();
    });
}

void SurveyEditor::deleteSurvey()
{
    if (!m_product.isValid())
        return;
    // TODO safety check
    const auto selection = ui->surveyView->selectionModel()->selectedRows();
    if (selection.isEmpty())
        return;
    const auto survey = selection.first().data(SurveyModel::SurveyRole).value<Survey>();
    if (survey.id() < 0)
        return;
    auto reply = RESTApi::deleteSurvey(m_restClient, survey);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            return;
        emit logMessage(QString::fromUtf8(reply->readAll()));
        m_surveyModel->reload();
    });
}
