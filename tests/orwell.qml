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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import Qt.labs.settings 1.0
import org.kde.userfeedback 1.0 as UserFeedback

ApplicationWindow {
    id: root
    width: 480
    height: 640

    Component.onCompleted: {
        Qt.application.name = "qmlorwell";
        Qt.application.version = "1984.qml"
        Qt.application.organization = "KDE";
        Qt.application.domain = "kde.org";

        // for Settings to work, we need to have the above setup first
        stackView.push(mainView);
    }

    UserFeedback.Provider {
        id: provider
        submissionInterval: 1
        productIdentifier: "org.kde.orwell"
        feedbackServer: "https://feedback.volkerkrause.eu/"
        applicationStartsUntilEncouragement: 5
        encouragementDelay: 10
        encouragementInterval: 1
        // TODO make configurable
        statisticsCollectionMode: UserFeedback.Provider.DetailedUsageStatistics
        surveyInterval: 0

        onSurveyAvailable: {
            console.log(survey);
            surveyPopup.surveyInfo = survey;
            surveyPopup.open();
        }
        onShowEncouragementMessage: {
            console.log("showing encouragement");
            encouragementPopup.open();
        }

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

        UserFeedback.PropertyRatioSource {
            id: dialRatioSource
            mode: UserFeedback.Provider.DetailedUsageStatistics
            name: "dialRatio"
            propertyName: "intValue"
            description: qsTr("The dial position.")

            Component.onCompleted: {
                dialRatioSource.addValueMapping(0, "off");
                dialRatioSource.addValueMapping(11, "max");
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
    }

    Component {
        id: mainView
        Page {
            header: ToolBar {
                RowLayout {
                    anchors.fill: parent
                    Label {
                        text: "Feedback Demo"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        Layout.fillWidth: true
                    }
                    ToolButton {
                        id: menuButton
                        text: "⋮"
                        onClicked: menu.open()
                        Menu {
                            id: menu
                            y: menuButton.height
                            MenuItem {
                                text: "Contribute..."
                                onTriggered: stackView.push(contributePage)
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                anchors.fill: parent
                Dial {
                    property int intValue: Math.round(dial.value)
                    id: dial
                    Layout.fillWidth: true
                    from: 0
                    to: 11
                    stepSize: 1
                    Component.onCompleted: dialRatioSource.object = dial
                }
                Settings {
                    property alias dialValue: dial.value
                }
                Button {
                    id: submitButton
                    Layout.alignment: Qt.AlignHCenter
                    text: "Submit!"
                    onClicked: provider.submit()
                }
            }
        }
    }

    Component {
        id: contributePage
        Page {
            header: ToolBar {
                RowLayout {
                    anchors.fill: parent
                    ToolButton {
                        id: menuButton
                        text: "<"
                        onClicked: stackView.pop()
                    }
                    Label {
                        text: "Contribution Settings"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        Layout.fillWidth: true
                    }
                }
            }

        }
    }

    Popup {
        id: surveyPopup
        property var surveyInfo;
        x: 0
        y: 0
        width: parent.width
        height: parent.height

        ColumnLayout {
            anchors.fill: parent
            Label {
                Layout.fillWidth: true
                horizontalAlignment: Qt.AlignHCenter;
                text: qsTr("We are looking for your feedback!")
                font.bold: true
            }
            Label {
                Layout.fillWidth: true
                text: qsTr("We would like a few minutes of your time to provide feedback about this application in a survey.")
                wrapMode: Text.WordWrap
            }
            Button {
                Layout.alignment: Qt.AlignHCenter;
                text: qsTr("Participate!")
                onClicked: {
                    if (Qt.openUrlExternally(surveyPopup.surveyInfo.url))
                        provider.surveyCompleted(surveyPopup.surveyInfo);
                    surveyPopup.close()
                }
            }
        }
    }

    Popup {
        id: encouragementPopup
        x: 0
        y: 0
        width: parent.width
        height: parent.height

        ColumnLayout {
            anchors.fill: parent
            Label {
                text: qsTr("Help us make this application better!");
                Layout.fillWidth: true
                horizontalAlignment: Qt.AlignHCenter;
                font.bold: true
            }
            Label {
                Layout.fillWidth: true
                text: qsTr("You can help us improving this application by sharing statistics and participate in surveys.")
                wrapMode: Text.WordWrap
            }
            Button {
                Layout.alignment: Qt.AlignHCenter;
                text: qsTr("Contribute...")
                onClicked: {
                    encouragementPopup.close();
                    if (!contributePage.visible)
                        stackView.push(contributePage);
                }
            }
        }
    }
}
