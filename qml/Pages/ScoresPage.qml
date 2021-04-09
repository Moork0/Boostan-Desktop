import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import API.ScoresHandler 1.0
import "../Controls"

PageBase {
    id: scores_page

    ScoresHandler {
        id: scores_handler
//        Component.onCompleted: start()
    }

    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#262A2F"
    }

    MyButton {
        id: btn_select_semester
        anchors.right: parent.right
        anchors.rightMargin: 20
        y: 20
        width: 170
        height: 50
        // TODO: change text to a more meaningful text
        text: "برنامه ساخته شده"
        bgColor: "#19B99A"
        radius: 8
        font.family: regular_font.name
    }

//    Rectangle {
//        id: test
//        border.color: "red"
//        border.width: 2
//        width: parent.width - 40
//        height: parent.height - btn_select_semester.height - 20 - 40
//        anchors.top: btn_select_semester.bottom
//        anchors.topMargin: 10
//        anchors.horizontalCenter: parent.horizontalCenter
//    }

    ColumnLayout {
        id: layout
        width: parent.width - 40
        // 20 is the btn_select_semester.y and 40 is our specific margin
        height: parent.height - btn_select_semester.height - 20 - 40
        anchors.top: btn_select_semester.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.fill: test
        spacing: 0

        MyTableView {
            id: scores_table
            Layout.preferredWidth: parent.width * 0.9
//            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.8
            Layout.maximumHeight: parent.height * 0.75
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter

            choosable: false
            model: [
                {"name": "سلام", "weight": "3", "status": "اعلام شده"},
                {"name": "سلام", "weight": "3", "status": "اعلام شده"}
            ]

            columnKey: ["name", "weight", "status"]
            columnSizes: [0.5, 0.2, 0.3]
            columnTitle: ["اسم", "واحد", "وضعیت"]
        }

        MyTableView {
            id: brief_scores_table
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            Layout.alignment: Qt.AlignBottom

            choosable: false
            model: [
                {"name": "سلام", "weight": "3", "status": "اعلام شده"}
            ]

            columnKey: ["name", "weight", "status"]
            columnSizes: [0.5, 0.2, 0.3]
            columnTitle: ["اسم", "واحد", "وضعیت"]
        }

    }

}
