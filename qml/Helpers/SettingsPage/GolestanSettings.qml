import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "../../Controls"

Item {
    ColumnLayout {
        width: parent.width - 10
        height: parent.height - 20
        anchors.centerIn: parent
        spacing: 0
        MyTextInput {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.topMargin: 20
            width: 240
            height: 40
            Layout.preferredHeight: 40
            direction: Qt.RightToLeft
            placeHolder: "رمز عبور فعلی"
            mode: TextInput.Password
            icon: "\ue800" // profile icon
            iconSize: 24
        }

        MyTextInput {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            width: 240
            height: 40
            Layout.preferredHeight: 40
            direction: Qt.RightToLeft
            placeHolder: "رمز عبور جدید"
            mode: TextInput.Password
            icon: "\ue80d" // lock icon
            iconSize: 24
        }

        MyTextInput {
            id: newpass_re
            Layout.alignment: Qt.AlignHCenter
            width: 240
            height: 40
            Layout.preferredHeight: 40
            direction: Qt.RightToLeft
            placeHolder: "تکرار رمز عبور جدید"
            mode: TextInput.Password
            icon: "\ue80d" // lock icon
            iconSize: 24
        }

        Row {
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 50
            Layout.preferredHeight: 30
            Switch {
                id: change_un
                anchors.verticalCenter: parent.verticalCenter

            }

            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: "تغییر نام کاربری"
                color: "#FCFCFC"
            }
        }

        MyTextInput {
            visible: change_un.checked
            Layout.alignment: Qt.AlignHCenter
            width: 240
            height: 40
            Layout.preferredHeight: 40
            direction: Qt.RightToLeft
            placeHolder: "نام کاربری جدید"
            mode: TextInput.Password
            icon: "\ue805" // person icon
            iconSize: 24
        }

        MyButton {
            id: submit_button
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 240
            Layout.preferredHeight: 50
            text: "ثبت"
            color: "#FCFCFC"
            bgColor: "#19B99A"
            radius: 5
            font.pixelSize: 15
            font.family: regular_font.name
            font.weight: Font.Bold
        }
    }
}
