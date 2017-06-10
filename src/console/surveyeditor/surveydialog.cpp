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

#include <config-userfeedback.h>

#include "surveydialog.h"
#include "ui_surveydialog.h"

#include <core/survey.h>

#include <common/surveytargetexpressionparser.h>

#include <QIcon>
#include <QPushButton>
#include <QUrl>
#include <QUuid>

using namespace KUserFeedback::Console;

SurveyDialog::SurveyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurveyDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme(QStringLiteral("dialog-question")));

    connect(ui->buttonBox->button(QDialogButtonBox::Discard), &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->name, &QLineEdit::textChanged, this, &SurveyDialog::updateState);
    connect(ui->url, &QLineEdit::textChanged, this, &SurveyDialog::updateState);
    connect(ui->targetExpression, &QPlainTextEdit::textChanged, this, &SurveyDialog::updateState);
    updateState();
}

SurveyDialog::~SurveyDialog() = default;

Survey SurveyDialog::survey() const
{
    Survey s = m_survey;
    s.setName(ui->name->text());
    s.setUrl(QUrl(ui->url->text()));
    s.setTarget(ui->targetExpression->toPlainText());
    if (s.uuid().isNull())
        s.setUuid(QUuid::createUuid());
    return s;
}

void SurveyDialog::setSurvey(const Survey& survey)
{
    m_survey = survey;
    ui->name->setText(survey.name());
    ui->url->setText(survey.url().toString());
    ui->targetExpression->setPlainText(survey.target());
    updateState();
}

void SurveyDialog::updateState()
{
    bool valid = true;
    valid &= !ui->name->text().isEmpty();
    const auto url = QUrl(ui->url->text());
    valid &= url.isValid() && !url.scheme().isEmpty() && !url.host().isEmpty();

#ifdef HAVE_SURVEY_TARGET_EXPRESSIONS
    if (!ui->targetExpression->toPlainText().isEmpty()) {
        SurveyTargetExpressionParser p;
        valid &= p.parse(ui->targetExpression->toPlainText());
    }
#endif

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(valid);
}
