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

#include "surveydialog.h"
#include "ui_surveydialog.h"

#include <core/survey.h>

#include <QIcon>
#include <QPushButton>
#include <QUrl>

using namespace UserFeedback::Console;

SurveyDialog::SurveyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurveyDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme(QStringLiteral("dialog-question")));

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &QDialog::reject);
}

SurveyDialog::~SurveyDialog() = default;

Survey SurveyDialog::survey() const
{
    Survey s = m_survey;
    s.setName(ui->name->text());
    s.setUrl(QUrl(ui->url->text()));
    s.setTarget(ui->targetExpression->toPlainText());
    return s;
}

void SurveyDialog::setSurvey(const Survey& survey)
{
    m_survey = survey;
    ui->name->setText(survey.name());
    ui->url->setText(survey.url().toString());
    ui->targetExpression->setPlainText(survey.target());
}
