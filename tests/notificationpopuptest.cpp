/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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
