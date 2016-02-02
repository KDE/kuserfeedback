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

#include <provider/core/provider.h>
#include <provider/core/surveyinfo.h>

#include <QApplication>
#include <QDesktopServices>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char** argv)
{
    QCoreApplication::setApplicationName(QStringLiteral("orwell"));
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1984"));

    QApplication app(argc, argv);

    UserFeedback::Provider provider;
    provider.setProductIdentifier(QStringLiteral("org.kde.orwell"));
    provider.setFeedbackServer(QUrl(QStringLiteral("https://feedback.volkerkrause.eu")));

    QObject::connect(&provider, &UserFeedback::Provider::surveyAvailable, &app, [&provider](const UserFeedback::SurveyInfo &info) {
        QDesktopServices::openUrl(info.url());
        provider.setSurveyCompleted(info);
    });

    // TODO
    QWidget top;
    top.show();

    auto topLayout = new QVBoxLayout;
    top.setLayout(topLayout);

    auto submitButton = new QPushButton(QStringLiteral("Submit Manually"));
    topLayout->addWidget(submitButton);
    QObject::connect(submitButton, SIGNAL(clicked()), &provider, SLOT(submit()));

    return app.exec();
}
