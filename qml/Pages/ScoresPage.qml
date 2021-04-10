import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import API.ScoresHandler 1.0
import "../Controls"

PageBase {
    id: scores_page

    ScoresHandler {
        id: scores_handler
        Component.onCompleted: {
            start()
            scores_table.model = getScores();
        }

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
            Layout.preferredHeight: parent.height * 0.8
            Layout.maximumHeight: parent.height * 0.75
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter

            choosable: false
            model: []
            columnKey: ["name", "weight", "score", "status"]
            columnSizes: [0.4, 0.15, 0.2, 0.2]
            columnTitle: ["اسم", "واحد","نمره", "وضعیت"]
            columnItem: scores_table_cell
        }

        MyTableView {
            id: brief_scores_table
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            Layout.alignment: Qt.AlignBottom

            choosable: false
            model: []
            columnKey: ["name", "weight", "status"]
            columnSizes: [0.5, 0.2, 0.3]
            columnTitle: ["اسم", "واحد", "وضعیت"]
        }

    }

    Component {
        id: scores_table_cell
        MyTableView.BaseColumnItem {
            property var model_text: model[role]
            property var text_color: "#FFFFFF"
            Component.onCompleted: {
                if (model.status === ScoresHandler.Deleted) {
                    model_text = role === "status" ? "حذف شده" : model[role]
                    text_color = "#757575"
                    return;
                }

                if (role === "score" || role === "status") {
                    if (model.status === ScoresHandler.Passed) {
                        model_text = role === "score" ? model.score : "قبول"
                        text_color = "#22E533"
                    } else if (model.status === ScoresHandler.Failed) {
                        model_text = role === "score" ? model.score : "رد"
                        text_color = "#E25D5D"
                    } else if (model.status === ScoresHandler.Temporary) {
                        model_text = role === "score" ? model.score : "موقت"
                        text_color = "#F4FC7C"
                    } else if (model.status === ScoresHandler.Undefined) {
                        model_text = role === "score" ? model.score : "نامشخص"
                    }
                }
            }

            Label {
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: regular_font.name
                text: parent.model_text
                color: parent.text_color
            }
        }
    }

}
