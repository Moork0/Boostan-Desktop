import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls.Material 2.15
import API.InitHandler 1.0
import API.LoginHandler 1.0

Page {
    id: login_page

    InitHandler {
        id: init_handler
        Component.onCompleted: start();
        onFinished: console.log("finished with status: " + init_handler.success)
    }

    LoginHandler {
        id: login_handler
    }

    Item {
        anchors.fill: parent
        id: page_background

        Image {
            id: background_image
            anchors.fill: parent
            source: "pics/LoginBackground.svg"
            visible: false
        }

        FastBlur {
            anchors.fill: background_image
            id: blur_bg_image
            source: background_image
            radius: 32
            opacity: 0.9
        }

//        Label {
//            anchors.centerIn: parent
//            text: "string is" + init_handler.errorString
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    init_handler.start()
//                }
//            }
//        }
    }

    Label {
        id: header_name
        anchors.horizontalCenter: form_container.horizontalCenter
        anchors.bottom: form_container.top
        anchors.bottomMargin: 20
        text: "بوستان"
        font.bold: Font.Bold
        font.pixelSize: 40
    }

    Rectangle {
        id: form_container
        anchors.centerIn: parent
        width: 300
        height: 300
        radius: 20
        color: Qt.rgba(255, 255, 255, 0.5)
        opacity: 0.6
//        visible: false
    }
//    DropShadow {
//        anchors.fill: form_container
//        source: form_container
//        horizontalOffset: 5
//        verticalOffset: 5
//        radius: 4
//        samples: 9
//        color: Qt.rgba(255, 255, 255, 0.5)
//        opacity: 0.6
//    }

    MyTextInput {
        visible: false
        id: test
        width: 240
        height: 40
        anchors.centerIn: form_container
        direction: Qt.RightToLeft
        placeHolder: "نام کاربری"
        icon: "\ue805" // profile icon
    }
}
