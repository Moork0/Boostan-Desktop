import QtQuick 2.15
import QtQuick.Controls 2.15
import "../../Controls"

Item {

    Column {
        width: parent.width - 40
        height: parent.height - 40
        anchors.centerIn: parent
        spacing: 10
        Label {
            id: about_boostan
            width: parent.width
            wrapMode: Label.WordWrap
            color: "#FCFCFC"
            font.family: regular_font.name
            font.pixelSize: 16
            text: "بوستان یک کارخواه(کلاینت) آزاد برای سامانهٔ دانشگاهی گلستان است."
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#262A2F"
        }

        Label {
            width: parent.width
            wrapMode: Label.WordWrap
            color: "#FCFCFC"
            font.family: regular_font.name
            font.pixelSize: 16
            text: "نسخه: " + Version
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#262A2F"
        }

        Label {
            width: parent.width
            wrapMode: Label.WordWrap
            color: "#FCFCFC"
            font.family: regular_font.name
            font.pixelSize: 16
            text: "برای اطلاعات بیشتر روی لینک کلیک کنید"
        }

        ClickableText {
            width: parent.width
            wrapMode: Label.WordWrap
            font.family: regular_font.name
            font.pixelSize: 16
            text: "<a href='https://seedpuller.github.io/Boostan-Desktop/'>https://seedpuller.github.io/Boostan-Desktop/</a>"
            onClicked: {
                Qt.openUrlExternally("https://seedpuller.github.io/Boostan-Desktop/")
            }
        }

    }
}
