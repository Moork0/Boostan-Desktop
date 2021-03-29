/*
    * This is Login Page.
    * In this page the first request will being sent(with init handler)
    * This page has no right_pane so the right_pane should be disabled when this page is visible.
*/

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts 1.15
import API.InitHandler 1.0
import API.LoginHandler 1.0
import API.Settings 1.0
import "../Controls"

PageBase {
    id: login_page

    InitHandler {
        id: init_handler
        Component.onCompleted: start();
        onFinished: {
            if (init_handler.success)
                captcha_handler.loadCaptcha(captcha_pic)
            else
                error_handler.raiseError(this, function() {init_handler.start()})
        }
    }

    LoginHandler {
        id: login_handler
        onFinished: {
            // login handler job is finished so the submit_loading should not be visible
            submit_loading.visible = false
            if (login_handler.success) {
                if (remember_checkbox.checked) {
                    Settings.setValue("username", username_input.text)
                    Settings.setValue("password", password_input.text)
                }
                universal_storage.studentName = getName()
                right_pane.enabled = true;
                // redirect to dashboard page and don't cache current page
                right_pane.toOption(0, false);
                return;
            }
            error_handler.raiseError(this, function(){init_handler.start()}, notifier)
        }
    }

    CaptchaHandler {
        id: captcha_handler
        onFinished: {
            if(captcha_handler.success) {
                captcha_pic.source = "file:/" + ApplicationPath + "captcha.png"
                return
            }
            error_handler.raiseError(this, function(){captcha_handler.loadCaptcha(captcha_pic)}, notifier)
        }


        function loadCaptcha(cpic) {
            cpic.source = "file:/" + ApplicationPath + "/pic/captcha.png"
            getCaptcha()
        }
    }

    Rectangle {
        id: form_background
        anchors.fill: parent
        color: "#262125"

        Image {
            anchors.bottom: form_container_bg.top
            anchors.bottomMargin: -135
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/pics/boostan-logo.svg"
            width: 400
            height: 320
            z: 1
        }

        Rectangle {
            id: form_container_bg
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 0.35
            height: parent.height * 0.75
            color: "#19B99A"
            gradient: Gradient {
                GradientStop { color: "#262125"; position: 0.0 }
                GradientStop { color: "#19B99A"; position: 0.7 }
            }
        }

        Rectangle {
            id: form_container
            anchors.centerIn: form_container_bg
            width: 320
            height: 320
            radius: 20
            color: "#424242"
        }

        ColumnLayout {
            id: form_layout
            anchors.fill: form_container
            spacing: 0
            Item {
                Layout.preferredHeight: 10
            }

            MyTextInput {
                id: username_input
                Layout.alignment: Qt.AlignHCenter
                width: 240
                height: 40
                direction: Qt.RightToLeft
                placeHolder: "نام کاربری"
                icon: "\ue805" // profile icon
                text: Settings.getValue("username") === undefined ? "" : Settings.getValue("username")
            }

            MyTextInput {
                id: password_input
                Layout.alignment: Qt.AlignHCenter
                width: 240
                height: 40
                direction: Qt.RightToLeft
                placeHolder: "رمز عبور"
                mode: TextInput.Password
                icon: "\ue800" // profile icon
                iconSize: 24
                text: Settings.getValue("password") === undefined ? "" : Settings.getValue("password")
            }

            /*
                * Captcha Layout
            */
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.maximumWidth: 240

                Icon {
                    id: reload_icon
                    Layout.leftMargin: -15
                    color: "#EEEEEE"
                    text: "\ue801" // reload icon
                    clickAble: true
                    onClicked: {
                        rotate.start()
                        captcha_handler.loadCaptcha(captcha_pic)
                    }

                    RotationAnimation {
                        id: rotate
                        target: reload_icon
                        from: 0
                        to: 360
                        duration: 400
                        easing.type: Easing.InOutQuad
                    }

                }

                Image {
                    id: captcha_pic
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 40
                    cache: false
                    source: "file:/" + ApplicationPath + "/pic/captcha.png"
                    LoadingAnimationColor {
                        id: captcha_loading
                        anchors.fill: captcha_pic
                        visible: captcha_handler.working
                    }
                }

                MyTextInput {
                    id: captcha_input
                    Layout.fillWidth: true
                    height: 40
                    placeHolder: "تصویر امنیتی"
                    horizontalAlignment: TextInput.AlignHCenter
                }
            }

            /*
                 *  Remember me Layout
            */
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.maximumWidth: 240
                Layout.maximumHeight: 20
                Layout.bottomMargin: 20
                Layout.topMargin: -10
                layoutDirection: Qt.RightToLeft
                spacing: 0
                CheckBox {
                    id: remember_checkbox
                    Layout.preferredWidth: 15
                }
                Label {
                    Layout.rightMargin: -15
                    text: "مرا به خاطر بسپار"
                    font.weight: Font.Bold
                    color: "#EEEEEE"
                    font.family: regular_font.name
                }
            }

            MyButton {
                id: submit_button
                enabled: captcha_handler.finished && captcha_handler.success
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 240
                Layout.preferredHeight: 50
                text: "ورود"
                bgColor: "#19B99A"
                radius: 5
                font.pixelSize: 15
                font.family: regular_font.name
                onClicked: {
                    if (username_input.isEmpty || password_input.isEmpty || captcha_input.isEmpty) {
                        notifier.text = "ورودی ها نباید خالی باشن!"
                        notifier.solution = "یک بار دیگه فرم رو بررسی کن و همه ورودی هارو پر کن"
                        notifier.show()
                        return
                    }
                    login_handler.tryLogin(username_input.text, password_input.text, captcha_input.text)
                    submit_loading.visible = true
                }

                Rectangle {
                    // the visiblity of this component is managed by
                    // login_handler.onFinished and submit_button.onCLicked
                    id: submit_loading
                    visible: false
                    anchors.fill: submit_button
                    color: "#19B99A"
                    LoadingAnimationPulse {
                        anchors.centerIn: parent
                        running: parent.visible
                        barCount: 3
                        color: "#FAFAFA"
                        width: 40
                        height: 25
                    }
                    // TODO: check if there is a better way to disable the mouse interaction of button
                    // when button is in the Loading state
                    MouseArea {
                        anchors.fill: parent
                    }
                }
            }

            Item {
                Layout.preferredHeight: 10
            }
        }
    }

    Notifier {
        id: notifier
        anchors.horizontalCenter: parent.horizontalCenter
        bgColor: "#262125"
        font.family: regular_font.name
    }
}
