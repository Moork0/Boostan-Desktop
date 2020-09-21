import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: error_page
    property alias error_msg: error_text.text
    property alias error_solution: error_solution.text
    property var callback_function
    property bool isCritical: false

    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#FAFAFA"
        Label {
            id: eror_icon
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 120
            text: "\ue802" // sad icon
            font.family: "fontello"
            font.pixelSize: error_page.width / 5
            color: "#424242"
        }

        Label {
            id: error_text
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: eror_icon.bottom
            anchors.topMargin: 40
            font.family: "Tanha"
            font.pixelSize: eror_icon.font.pixelSize / 5
//            text: "این یک متن ارور است"
        }

        Label {
            id: error_solution
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: error_text.bottom
            font.family: "Tanha"
            font.pixelSize: error_text.font.pixelSize / 2
//            text: "این یک توضیحات اضافه است"
        }

        MyButton {
            id: retry_button
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: error_solution.bottom
            anchors.topMargin: 10
            width: 150
            height: 50
            // this text seems reverse. correct = if critical, close page. else try again
            text: error_page.isCritical ? "بستن برنامه!" : "دوباره تلاش کن!"
            bgColor: error_page.isCritical ? "#E53935" :  "#ED960B"
            radius: 5
            onClicked: {
                if (error_page.isCritical) {
                    Qt.quit()
                    return;
                }
                callback_function()
            }

            font.pixelSize: 15
        }
    }
}
