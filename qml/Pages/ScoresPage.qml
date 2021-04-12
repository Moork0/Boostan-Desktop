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
            brief_scores_table.model = getBriefScores()
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

    ColumnLayout {
        id: layout
        width: parent.width - 40
        // 20 is the btn_select_semester.y and 40 is our specific margin
        height: parent.height - btn_select_semester.height - 30 - 40
        anchors.top: btn_select_semester.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.fill: test
        spacing: 0

        MyTableView {
            id: scores_table
            Layout.preferredWidth: parent.width * 0.85
            Layout.preferredHeight: parent.height * 0.8
            Layout.maximumHeight: parent.height * 0.8
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter

            choosable: false
            model: []
            columnKey: ["name", "weight", "score", "status"]
            columnSizes: [0.5, 0.1, 0.15, 0.25]
            columnTitle: ["درس", "واحد","نمره", "وضعیت"]
            columnItem: scores_table_cell
            rowHeight: 55
        }

        MyTableView {
            id: brief_scores_table
//            Layout.fillWidth: true
            Layout.preferredWidth: parent.width * 0.9
            Layout.preferredHeight: 100
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter

            interactive: false
            choosable: false
            model: []
            columnKey: ["average", "semesterUnits", "passedUnits", "passedUnits", "passedUnits"]
            columnSizes: [0.2, 0.2, 0.2, 0.15, 0.25]
            columnTitle: ["معدل این ترم", "چند واحد گرفتی", "چند واحد پاس شدی", "معدل کل", "کلا چند واحد پاس کردی"]
            rowHeight: 50
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
                } else if (model.status === ScoresHandler.Passed) {
                    model_text = role === "status" ? "قبول" : model[role]
                    text_color = "#22FF32"
                } else if (model.status === ScoresHandler.Failed) {
                    model_text = role === "status" ? "مردود" : model[role]
                    text_color = "#FF6363"
                } else if (model.status === ScoresHandler.Temporary) {
                    model_text = role === "status" ? "موقت" : model[role]
                    text_color = "#F7FF7D"
                } else if (model.status === ScoresHandler.Undefined) {
                    model_text = role === "status" ? "نامشخص" : model[role]
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
                font.weight: Font.DemiBold
            }
        }
    }

}
