import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import API.ScoresHandler 1.0
import "../Controls"
import "../Helpers"

PageBase {
    id: scores_page

    ScoresHandler {
        id: scores_handler

        Component.onCompleted: {
            right_pane.disableNavigator()

            // model for using in 'btn_select_semester'
            var combo_model = []
            var currentSem = universal_storage.currentSemester
            var currentSemText = "نیمسال " + Number(currentSem % 1000).toLocaleString(Qt.locale("fa_IR"), "f", 0)

            for (var i = 0; i < universal_storage.semesters.length; ++i) {
                var sem_value = universal_storage.semesters[i];
                var sem_text = "نیمسال " + Number(sem_value % 1000).toLocaleString(Qt.locale("fa_IR"), "f", 0);
                combo_model.push({"value": sem_value, "text": sem_text})
            }
            combo_model.push({"value": currentSem, "text": currentSemText})
            btn_select_semester.model = combo_model;
            btn_select_semester.currentIndex = universal_storage.semesters.length

            start(currentSem, universal_storage.studentUid)
        }

        onFinished: {
            right_pane.enableNavigator()
            if (!success) {
                error_handler.raiseError(this, function(){offered_course_handler.start()}, notifier)
                return;
            }

            scores_table.model = getScores();
            brief_scores_table.model = getBriefScores()
        }

    }

    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#262A2F"
    }

    Notifier {
        id: notifier
        showType: Notifier.ShowType.LeftToRight
        anchors.top: parent.top
        anchors.topMargin: 50
        z: 2
        font.family: regular_font.name
        bgColor: "#E65100"
    }

    MyComboBox {
        id: btn_select_semester
        anchors.right: parent.right
        anchors.rightMargin: 20
        y: 20
        width: 170
        height: 60
        popupMaxHeight: 200
        comboItem.textRole: "text"
        comboItem.valueRole: "value"

        model: 0

        onActivated: {
            scores_handler.getScoresOf(currentValue)
        }

    }

    ColumnLayout {
        id: layout
        visible: scores_handler.finished
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
            autoHeight: true
//            Layout.preferredHeight: parent.height * 0.8
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
            columnKey: ["average", "semesterUnits", "passedUnits", "totalAvg", "totalPassedUnits"]
            columnSizes: [0.2, 0.2, 0.2, 0.15, 0.25]
            columnTitle: ["معدل این ترم", "چند واحد گرفتی", "چند واحد پاس شدی", "معدل کل", "کلا چند واحد پاس کردی"]
            rowHeight: 50

        }

    }

    Item {
        width: layout.width
        height: layout.height
        x: layout.x
        y: layout.y
        LoadingAnimationColor {
            id: scores_loading_anim
            width: parent.width
            height: parent.height * 0.70
            radius: 5
            visible: !scores_handler.finished
        }
        ErrorRectangle {
            id: scores_err_rec
            visible: scores_handler.finished && scores_handler.is_empty
            anchors.fill: this
            width: parent.width
            height: parent.height * 0.70
            name: "نمرات"
            radius: 5
        }

        LoadingAnimationColor {
            y: scores_loading_anim.height + 40
            id: briefscores_loading_anim
            width: parent.width
            height: parent.height * 0.20
            radius: 5
            visible: scores_loading_anim.visible
        }
        ErrorRectangle {
            visible: scores_err_rec.visible
            anchors.fill: this
            y: scores_loading_anim.height + 50
            width: parent.width
            height: parent.height * 0.25
            name: "نیمسال"
            radius: 5
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
