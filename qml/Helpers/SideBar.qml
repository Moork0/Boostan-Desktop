/*
    * Side bar component
    * Show list of options (objects in 'options') and show them by click.
*/

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

Rectangle {
    id: side_bar
    color: "#232429"
    anchors.left: stackview.right
    width: enabled ? 270 : 0
    height: mainwindow.height

    // List of options which are going to be shown.
    default property list<SideBarItem> options

    property string studentName: universal_storage.studentName
    property real itemSize: width * 0.06
    // Index of current option in 'options'
    property int currentOption: -1
    property bool disableNav: false

    /** Functions **/

    function toOption(option, cache_current = true) {
        if (currentOption < 0) {
            currentOption = option
        } else {
            repeater.itemAt(currentOption).isEnable = false
            currentOption = option
        }
        repeater.itemAt(option).isEnable = true

        stackview.showPage(side_bar.options[option].componentPath, side_bar.options[option].componentPath, cache_current)
    }

    function enableNavigator()
    {
        side_bar.disableNav = false
    }

    function disableNavigator()
    {
        side_bar.disableNav = true
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        width: parent.width - side_bar_right.width + 5
        height: parent.height
        spacing: 0
        Label {
            Layout.alignment: Qt.AlignHCenter
            Layout.leftMargin: 10
            font.family: "Mj_Afsoon"
            font.pixelSize: 55
            color: "#19B99A"
            text: "بوستان"
        }

        Item {
            height: 18
        }

        Label {
            id: student_name
            Layout.alignment: Qt.AlignHCenter
            font.family: regular_font.name
            font.pixelSize: 12
            color: "#F8F7F2"
            text: side_bar.studentName
        }

        Label {
            id: today_date
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10
            text: TodayDate
            font.pixelSize: 12
            font.family: regular_font.name
            color: "#F8F7F2"
        }

        Item {
            height: 20
        }

        // Options shown here
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Column {
                width: parent.width
                height: parent.height
                Repeater {
                    id: repeater
                    model: options
                    delegate: SideBarDelegate {
                        width: parent.width
                        height: 40
                        onClicked: side_bar.toOption(index)
                        Label {
                            anchors.right: parent.right
                            anchors.rightMargin: 30
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.title
                            color: parent.isEnable ? "#19B99A" : "#F8F7F2"
                            font.family: regular_font.name
                            font.pixelSize: itemSize
                            font.bold: true
                        }
                    }
                }
            }

            MouseArea {
                id: navigator_disabler
                enabled: side_bar.disableNav
                anchors.fill: parent
                cursorShape: enabled ? Qt.BusyCursor : Qt.ArrowCursor
                hoverEnabled: enabled
            }
        }
    }

    Rectangle {
        id: side_bar_right
        anchors.right: side_bar.right
        anchors.rightMargin: -7
        width: enabled ? 70 : 0
        height: side_bar.height
        radius: 10
        color: "#19B99A"
        ColumnLayout {
            anchors.right: parent.right
            anchors.rightMargin: 13
            anchors.top: parent.top
            anchors.topMargin: 15
            width: parent.width - 20
            Icon {
                id: logout_icon
                Layout.alignment: Qt.AlignRight
                font.pixelSize: 15
                font.weight: Font.Thin
                text: "\ue803" // logout icon
                description: "خروج"
                color: "#262125"
                clickAble: true
            }
            Item {
                height: 10
            }

            Item {
                Layout.alignment: Qt.AlignHCenter
                width: 35
                height: 35
                Rectangle {
                    id: attention_bg
                    width: 35
                    height: 35
                    radius: 17
                    color: "#ECEDFF"
                    opacity: 0.63
                }
                Icon {
                    text: "\ue804" // attention icon
                    anchors.centerIn: attention_bg
                    font.weight: Font.Thin
                    font.pixelSize: 17
                }
            }

            Item {
                Layout.alignment: Qt.AlignHCenter
                width: 35
                height: 35
                Rectangle {
                    id: attention_circle_bg
                    width: 35
                    height: 35
                    radius: 17
                    color: "#ECEDFF"
                    opacity: 0.63
                }
                Icon {
                    text: "\ue807" // attention circle icon
                    anchors.centerIn: attention_circle_bg
                    font.weight: Font.Thin
                    font.pixelSize: 17
                }
            }
        }
    }
}
