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

Page {
    id: login_page

    InitHandler {
        id: init_handler
        Component.onCompleted: start();
        onFinished: init_handler.success ? captcha_handler.loadCaptcha(captcha_pic) : error_handler.raiseError(
        errorCode,
        function() {
            init_handler.start()
        }
        )
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
                stackview.replace("DashboardPage.qml", StackView.PushTransition)
                right_pane.studentName = getName()
                right_pane.enabled = true;
                right_pane.toOption(0);
                return;
            }
            error_handler.raiseError(login_handler.errorCode,
                                    function(){ init_handler.start() },
                                    notifier)
        }
    }

    CaptchaHandler {
        id: captcha_handler
        onFinished: {
            if(captcha_handler.success) {
                captcha_pic.source = "file:/" + ApplicationPath + "captcha.png"
                return
            }
            error_handler.raiseError(captcha_handler.errorCode,
                                    function(){ captcha_handler.loadCaptcha(captcha_pic) },
                                    notifier)
        }


        function loadCaptcha(cpic) {
            cpic.source = "file:/" + ApplicationPath + "/pic/captcha.png"
            getCaptcha()
        }
    }

    RowLayout {
        anchors.fill: parent
        id: page_background
        spacing: 0

        Rectangle {
            id: logo_background
            Layout.preferredWidth: parent.width / 2.5
            Layout.preferredHeight: parent.height
            color: "#19B99A"
            Image {
                anchors.centerIn: parent
                source: "pics/login-logo.svg"
                width: 330
                height: 200
            }
        }

        Rectangle {
            id: form_background
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            color: "#262125"

            Label {
                id: header_name
                anchors.horizontalCenter: form_container.horizontalCenter
                anchors.bottom: form_container.top
                anchors.bottomMargin: 20
                text: "بوستان"
                font.family: "Mj_Afsoon"
        //        font.bold: Font.Bold
                font.pixelSize: 60
                color: "#19B99A"
            }

            Rectangle {
                id: form_container
                anchors.centerIn: parent
                width: 320
                height: 320
                radius: 20
                color: Qt.rgba(255, 255, 255, 0.5)
                opacity: 0.6
        //        visible: false
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
        //            Layout.topMargin: -15
                    width: 240
                    height: 40
                    direction: Qt.RightToLeft
                    placeHolder: "نام کاربری"
                    icon: "\ue805" // profile icon
                    text: Settings.getValue("username") === undefined ? "" : Settings.getValue("username")
                }

                MyTextInput {
                    id: password_input
        //            Layout.topMargin: -40
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

                    ClickableText {
                        id: reload_icon
                        Layout.leftMargin: -15
                        font.family: "fontello"
                        font.pixelSize: 15
                        text: "\ue801" // reload icon
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
                        /**** TODO: the visiblity of this component should be integrated with working() signal ****/
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
                        MouseArea {
                            anchors.fill: parent
                        }
                    }
                }

                Item {
                    Layout.preferredHeight: 10
                }

            }

            Notifier {
                id: notifier
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
