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
