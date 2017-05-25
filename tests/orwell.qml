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

import QtQuick 2.0
import QtQuick.Controls 2.1
import org.kde.userfeedback 1.0 as UserFeedback

ApplicationWindow {
    id: root
    width: 480
    height: 640

    UserFeedback.Provider {
        id: provider
        submissionInterval: 1
        productIdentifier: "org.kde.orwell"
        feedbackServer: "https://feedback.volkerkrause.eu/"
        statisticsCollectionMode: UserFeedback.Provider.DetailedUsageStatistics

        UserFeedback.ApplicationVersionSource { mode: UserFeedback.Provider.BasicSystemInformation }
        UserFeedback.CompilerInfoSource { mode: UserFeedback.Provider.BasicSystemInformation }
        UserFeedback.CpuInfoSource { mode: UserFeedback.Provider.DetailedSystemInformation }
        UserFeedback.LocaleInfoSource { mode: UserFeedback.Provider.DetailedSystemInformation }
        UserFeedback.OpenGLInfoSource{ mode: UserFeedback.Provider.DetailedSystemInformation }
        UserFeedback.PlatformInfoSource { mode: UserFeedback.Provider.BasicSystemInformation }
        UserFeedback.QtVersionSource { mode: UserFeedback.Provider.BasicSystemInformation }
        UserFeedback.ScreenInfoSource { mode: UserFeedback.Provider.DetailedSystemInformation }
        UserFeedback.StartCountSource { mode: UserFeedback.Provider.BasicUsageStatistics }
        UserFeedback.UsageTimeSource { mode: UserFeedback.Provider.BasicUsageStatistics }
    }

    Button {
        id: submitButton
        text: "Submit!"
        anchors.centerIn: parent
        onClicked: provider.submit()
    }
}
