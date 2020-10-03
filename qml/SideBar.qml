import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: side_bar
    color: "#262125"
    anchors.left: stackview.right
    width: enabled ? 270 : 0
    height: mainwindow.height
    radius: 17

    property real itemSize: width * 0.06
    property var options: ["پیشخوان", "دروس ارائه شده", "کارنامه", "حساب کاربری"]
    property int currentOption: -1

    function toOption(option) {
        if (currentOption < 0) {
            currentOption = option
        } else {
            repeater.itemAt(currentOption).isEnable = false
            currentOption = option
        }
        repeater.itemAt(option).isEnable = true
    }

    ColumnLayout {
        anchors.left: parent.left
//        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 20
        width: parent.width - side_bar_right.width + 5
        spacing: 0
        Label {
            Layout.alignment: Qt.AlignHCenter
            Layout.leftMargin: 10
            font.family: "Mj_Afsoon"
            font.pixelSize: 55
            color: "#ED960B"
            text: "بوستان"
        }

        Item {
            height: 18
        }

        Label {
            id: student_name
            Layout.alignment: Qt.AlignHCenter
            text: "محسنی حیدریان رنجبر حسینی سلام"
            font.family: "Sahel"
            font.pixelSize: 12
            color: "#F8F7F2"
        }

        Label {
            id: today_date
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10
            text: "۱۱ شهریور ۱۳۹۹"
            font.pixelSize: 12
            font.family: "Sahel"
            color: "#F8F7F2"
        }

        Item {
            height: 20
        }

        Repeater {
            id: repeater
            model: options
            delegate: SideBarItem {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                onClicked: side_bar.toOption(index)
                Label {
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData
                    color: parent.isEnable ? "#ED960B" : "#F8F7F2"
                    font.family: "Sahel"
                    font.pixelSize: itemSize
                    font.bold: true
                }
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
        color: "#ED960B"
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
