/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include <notificationpopup.h>
#include <provider.h>
#include <surveyinfo.h>

#include <QApplication>
#include <QPushButton>
#include <QStandardPaths>
#include <QUuid>
#include <QVBoxLayout>

using namespace KUserFeedback;

int main(int argc, char **argv)
{
    QStandardPaths::setTestModeEnabled(true);

    QCoreApplication::setApplicationName(QStringLiteral("notificationpopuptest"));
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1984"));

    QApplication app(argc, argv);

    Provider provider;

    QWidget topLevel;
    auto layout = new QVBoxLayout(&topLevel);
    auto button = new QPushButton(QStringLiteral("Show Encouragement"));
    layout->addWidget(button);
    QObject::connect(button, &QPushButton::clicked, &provider, &Provider::showEncouragementMessage);
    button = new QPushButton(QStringLiteral("New Survey Available"));
    layout->addWidget(button);
    QObject::connect(button, &QPushButton::clicked, &provider, [&provider]() {
        SurveyInfo info;
        info.setUuid(QUuid(QStringLiteral("{9e529dfa-0213-413e-a1a8-8a9cea7d5a97}")));
        info.setUrl(QUrl(QStringLiteral("https://www.kde.org/")));
        emit provider.surveyAvailable(info);
    });

    auto popup = new NotificationPopup(&topLevel);
    popup->setFeedbackProvider(&provider);

    topLevel.resize(1024, 786);
    topLevel.show();
    return app.exec();
}
