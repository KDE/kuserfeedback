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

#include <feedbackconfigdialog.h>

#include <applicationversionsource.h>
#include <platforminfosource.h>
#include <provider.h>
#include <qtversionsource.h>
#include <screeninfosource.h>

#include <QApplication>
#include <QStandardPaths>

using namespace KUserFeedback;

int main(int argc, char **argv)
{
    QStandardPaths::setTestModeEnabled(true);

    QCoreApplication::setApplicationName(QStringLiteral("feedbackconfigwidgettest"));
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1984"));

    QApplication app(argc, argv);

    Provider provider;
    provider.addDataSource(new ApplicationVersionSource, Provider::BasicSystemInformation);
    provider.addDataSource(new PlatformInfoSource, Provider::BasicSystemInformation);
    provider.addDataSource(new ScreenInfoSource, Provider::DetailedSystemInformation);
    provider.addDataSource(new QtVersionSource, Provider::DetailedSystemInformation);

    FeedbackConfigDialog w;
    w.setFeedbackProvider(&provider);
    w.show();
    return app.exec();
}
