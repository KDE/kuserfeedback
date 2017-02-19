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

#include "orwell.h"
#include "ui_orwell.h"

#include <provider/widgets/feedbackconfigdialog.h>
#include <provider/core/applicationversionsource.h>
#include <provider/core/platforminfosource.h>
#include <provider/core/propertyratiosource.h>
#include <provider/core/provider.h>
#include <provider/core/qtversionsource.h>
#include <provider/core/screeninfosource.h>
#include <provider/core/startcountsource.h>
#include <provider/core/surveyinfo.h>

#include <QApplication>
#include <QDesktopServices>
#include <QSettings>

static std::unique_ptr<UserFeedback::Provider> provider; // TODO make this nicer

Orwell::Orwell(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Orwell)
{
    ui->setupUi(this);
    ui->notificationWidget->setFeedbackProvider(provider.get());
    loadStats();

    connect(ui->version, &QLineEdit::textChanged, this, [this]() {
        QCoreApplication::setApplicationVersion(ui->version->text());
    });

    connect(ui->submitButton, &QPushButton::clicked, provider.get(), &UserFeedback::Provider::submit);
    connect(ui->overrideButton, &QPushButton::clicked, this, [this] (){
        writeStats();
        QMetaObject::invokeMethod(provider.get(), "load");
        loadStats();
    });

    connect(ui->surveyInterval, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int value) {
        provider->setSurveyInterval(value);
    });

    connect(ui->actionContribute, &QAction::triggered, this, [this]() {
        UserFeedback::FeedbackConfigDialog dlg(this);
        dlg.setFeedbackProvider(provider.get());
        dlg.exec();
    });

    connect(ui->actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);

    auto propertyMonitorSource = new UserFeedback::PropertyRatioSource(ui->dial, "value", QStringLiteral("dialRatio"));
    propertyMonitorSource->setDescription(QStringLiteral("The dial position."));
    propertyMonitorSource->addValueMapping(0, QStringLiteral("off"));
    propertyMonitorSource->addValueMapping(11, QStringLiteral("max"));
    provider->addDataSource(propertyMonitorSource, UserFeedback::Provider::AllStatistics);
}

Orwell::~Orwell() = default;

void Orwell::loadStats()
{
    ui->version->setText(QCoreApplication::applicationVersion());

    QSettings settings;
    ui->startCount->setValue(settings.value(QStringLiteral("UserFeedback/ApplicationStartCount")).toInt());
    ui->runtime->setValue(settings.value(QStringLiteral("UserFeedback/ApplicationTime")).toInt());
    ui->surveys->setText(settings.value(QStringLiteral("UserFeedback/CompletedSurveys")).toStringList().join(QStringLiteral(", ")));
    ui->surveyInterval->setValue(provider->surveyInterval());
}

void Orwell::writeStats()
{
    QSettings settings;
    settings.setValue(QStringLiteral("UserFeedback/ApplicationStartCount"), ui->startCount->value());
    settings.setValue(QStringLiteral("UserFeedback/ApplicationTime"), ui->runtime->value());
    settings.setValue(QStringLiteral("UserFeedback/CompletedSurveys"), ui->surveys->text().split(QStringLiteral(", ")));
}


int main(int argc, char** argv)
{
    QCoreApplication::setApplicationName(QStringLiteral("orwell"));
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1984"));

    QApplication app(argc, argv);

    provider.reset(new UserFeedback::Provider);
    provider->setFeedbackServer(QUrl(QStringLiteral("https://feedback.volkerkrause.eu")));
    provider->setSubmissionInterval(1);
    provider->setApplicationStartsUntilEncouragement(5);
    provider->setEncouragementDelay(10);
    provider->addDataSource(new UserFeedback::ApplicationVersionSource, UserFeedback::Provider::BasicStatistics);
    provider->addDataSource(new UserFeedback::PlatformInfoSource, UserFeedback::Provider::AllStatistics);
    provider->addDataSource(new UserFeedback::QtVersionSource, UserFeedback::Provider::BasicStatistics);
    provider->addDataSource(new UserFeedback::ScreenInfoSource, UserFeedback::Provider::AllStatistics);
    provider->addDataSource(new UserFeedback::StartCountSource, UserFeedback::Provider::BasicStatistics);

    Orwell mainWindow;
    mainWindow.show();

    return app.exec();
}
