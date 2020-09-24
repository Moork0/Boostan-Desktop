import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import QtQuick.Layouts 1.15
import API.InitHandler 1.0
import API.LoginHandler 1.0

Page {
    id: login_page

    InitHandler {
        id: init_handler
//        Component.onCompleted: start();
//        onFinished: init_handler.success ? {} : stackview.push("ErrorPage.qml",
//        {"error_msg": init_handler.errorString,
//         "error_solution": init_handler.errorSolution,
//         "callback_function": function() {
//             stackview.pop()
//             init_handler.start()
//         }
//        })
    }

    LoginHandler {
        id: login_handler
        onFinished: console.log("login finished with status: " + init_handler.success)

    }

    RowLayout {
        anchors.fill: parent
        id: page_background
        spacing: 0
//        Image {
//            id: background_image
//            anchors.fill: parent
//            source: "pics/LoginBackground.svg"
//            visible: false
//        }

//        FastBlur {
//            anchors.fill: background_image
//            id: blur_bg_image
//            source: background_image
//            radius: 32
//            opacity: 0.9
//        }
        Rectangle {
            id: logo_background
            Layout.preferredWidth: parent.width / 2.5
            Layout.preferredHeight: parent.height
            color: "#ED960B"
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
                font.family: "Tanha"
        //        font.bold: Font.Bold
                font.pixelSize: 60
                color: "#ED960B"
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
                        source: "file:/" + ApplicationPath + "/pic/captcha.png"
                    }

                    MyTextInput {
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
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: 240
                    Layout.preferredHeight: 50
                    text: "ورود"
                    bgColor: "#ED960B"
                    radius: 5
                    onClicked: console.log("cl")
                    font.pixelSize: 15
                }

                Item {
                    Layout.preferredHeight: 10
                }

            }

        }
    }




}
