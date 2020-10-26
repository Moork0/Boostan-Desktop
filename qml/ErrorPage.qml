import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: error_page
    property alias error_msg: error_text.text
    property alias error_solution: error_solution.text
    property var callback_function
    property int criticalStatus: ErrorHandler.SemiCritical

    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#262125"
        Image {
            id: error_logo
            width: parent.width / 1.5
            height: parent.height / 1.2
            source: "pics/error-logo.svg"
            anchors.centerIn: parent
        }
        Label {
            id: eror_icon
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: error_logo.top
            anchors.topMargin: 50
            text: "\ue802" // sad icon
            font.family: "fontello"
            font.pixelSize: error_logo.width / 4
            color: "#F8F7F2"
        }

        Label {
            id: error_text
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: eror_icon.bottom
            anchors.topMargin: 40
            font.family: "Tanha"
            font.pixelSize: eror_icon.font.pixelSize / 4
            font.weight: Font.DemiBold
//            text: "این یک متن ارور است"
            color: "#F8F7F2"
        }

        Label {
            id: error_solution
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: error_text.bottom
            font.family: "Tanha"
            font.pixelSize: error_text.font.pixelSize / 2
            font.weight: Font.DemiBold
//            text: "این یک توضیحات اضافه است"
            color: "#F8F7F2"
        }

        MyButton {
            id: retry_button
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: error_solution.bottom
            anchors.topMargin: 10
            width: 200
            height: 60
            // this text seems reverse. correct = if status == Critical, close page. else try again
            text: error_page.criticalStatus == ErrorHandler.Critical ? "بستن برنامه!" : "دوباره تلاش کن!"
            font.pixelSize: 20
            bgColor: error_page.criticalStatus == ErrorHandler.Critical ? "#E53935" :  "#19B99A"
            radius: 5
            onClicked: {
                if (error_page.criticalStatus == ErrorHandler.Critical) {
                    Qt.quit()
                    return;
                }
                callback_function()
            }

        }
    }
}
