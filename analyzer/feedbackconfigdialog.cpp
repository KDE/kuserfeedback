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

#include "feedbackconfigdialog.h"
#include "ui_feedbackconfigdialog.h"

#include <provider/core/provider.h>

#include <QDebug>
#include <QDesktopServices>
#include <QPushButton>
#include <QUrl>

using namespace UserFeedback::Analyzer;

FeedbackConfigDialog::FeedbackConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeedbackConfigDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Contribute!"));
    ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("No, I do not want to contribute."));

    connect(ui->basicStats, &QCheckBox::toggled, this, &FeedbackConfigDialog::updateButtonState);
    connect(ui->advancedStats, &QCheckBox::toggled, this, &FeedbackConfigDialog::updateButtonState);
    connect(ui->basicSurveys, &QCheckBox::toggled, this, &FeedbackConfigDialog::updateButtonState);
    connect(ui->allSurveys, &QCheckBox::toggled, this, &FeedbackConfigDialog::updateButtonState);

    connect(ui->introLabel, &QLabel::linkActivated, this, &FeedbackConfigDialog::linkActivated);
    connect(ui->basicStatsLabel, &QLabel::linkActivated, this, &FeedbackConfigDialog::linkActivated);
    connect(ui->advancedStatsLabel, &QLabel::linkActivated, this, &FeedbackConfigDialog::linkActivated);
    connect(ui->basicSurveyLabel, &QLabel::linkActivated, this, &FeedbackConfigDialog::linkActivated);
    connect(ui->allSurveyLabel, &QLabel::linkActivated, this, &FeedbackConfigDialog::linkActivated);
}

FeedbackConfigDialog::~FeedbackConfigDialog() = default;

void FeedbackConfigDialog::setFeedbackProvider(UserFeedback::Provider* provider)
{
    m_provider = provider;
    ui->allSurveys->setChecked(m_provider->surveyInterval() >= 0 && m_provider->surveyInterval() < 90);
    ui->basicSurveys->setChecked(m_provider->surveyInterval() >= 90);
    updateButtonState();
}

void FeedbackConfigDialog::accept()
{
    qDebug();
    if (!ui->basicSurveys->isChecked()) {
        m_provider->setSurveyInterval(-1);
    } else {
        m_provider->setSurveyInterval(ui->allSurveys->isChecked() ? 0 : 90);
    }

    // TODO store provider settings, to not lose this on a crash

    QDialog::accept();
}

void FeedbackConfigDialog::linkActivated(const QString& link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void FeedbackConfigDialog::updateButtonState()
{
    const auto advStats = ui->advancedStats->isChecked();
    if (advStats)
        ui->basicStats->setChecked(true);
    ui->basicStats->setEnabled(!advStats);

    const auto allSurveys = ui->allSurveys->isChecked();
    if (allSurveys)
        ui->basicSurveys->setChecked(true);
    ui->basicSurveys->setEnabled(!allSurveys);


    const auto any = ui->basicStats->isChecked() || ui->basicSurveys->isChecked();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setVisible(any);
    ui->buttonBox->button(QDialogButtonBox::Close)->setVisible(!any);
}
