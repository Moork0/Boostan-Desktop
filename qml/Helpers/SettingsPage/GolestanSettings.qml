import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import API.Settings 1.0
import API.AccountHandler 1.0

import "../../Controls"

Item {

    AccountHandler {
        id: account_handler
        onFinished: {
            right_pane.enableNavigator()
            if (!success) {
                error_handler.raiseError(this, function(){}, notifier)
                return;
            }
            notifier.text = "اطلاعات با موفقیت تغییر پیدا کردند."
            notifier.solution = "این موفقیت رو به شما تبریک عرض می‌نماییم"
            notifier.show()
        }
    }

    ColumnLayout {
        width: parent.width - 10
        height: parent.height - 20
        anchors.centerIn: parent
        spacing: 0
        MyTextInput {
            id: currpass_inp
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
            id: newpass_inp
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
            id: newpass_re_inp
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
            MySwitch {
                id: change_un
                anchors.verticalCenter: parent.verticalCenter
                width: 110
                height: 35
            }

            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: "تغییر نام کاربری"
                color: "#FCFCFC"
            }
        }

        MyTextInput {
            id: new_username_inp
            visible: change_un.checked
            Layout.alignment: Qt.AlignHCenter
            width: 240
            height: 40
            Layout.preferredHeight: 40
            direction: Qt.RightToLeft
            placeHolder: "نام کاربری جدید"
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
            onClicked:  {

                if (currpass_inp.isEmpty || newpass_inp.isEmpty || newpass_re_inp.isEmpty || (new_username_inp.isEmpty && change_un.checked)) {
                    notifier.text = "ورودی ها نباید خالی باشن!"
                    notifier.solution = "یک بار دیگه فرم رو بررسی کن و همه ورودی هارو پر کن"
                    notifier.show()
                    return;
                }

                if (newpass_inp.text !== newpass_re_inp) {
                    notifier.text = "رمز های جدید باهم مطابقت ندارن!"
                    notifier.solution = "رمز جدید و تکرارش رو بررسی کن"
                    notifier.show()
                    return;
                }

                if (currpass_inp.text === newpass_inp.text) {
                    notifier.text = "نمیشه که رمز فعلیتو نباید بجای رمز جدید وارد کنی!"
                    notifier.solution = "نمیشه که رمز فعلی‌ت رو بجای رمز فعلی‌ت بذاری آخه :))"
                    notifier.show()
                    return;
                }

                right_pane.disableNavigator()

                if (change_un.checked)
                    account_handler.changeCreds(universal_storage.username, currpass_inp.text, newpass_inp.text)
                else
                    account_handler.changeCreds(universal_storage.username, currpass_inp.text, newpass_inp.text, new_username_inp.text)

            }
        }
    }
}
