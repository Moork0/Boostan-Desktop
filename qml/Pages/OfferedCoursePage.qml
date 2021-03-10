import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"
import "../Helpers"
import API.OfferedCourseHandler 1.0
import API.Settings 1.0

PageBase {
    id: offered_course_page

    OfferedCourseHandler {
        id: offered_course_handler
        Component.onCompleted: start()
        onFinished: {
            sendDataTo(offered_course_model)
            table_view.model = offered_course_model
        }
    }

    OfferedCourseModel {
        id: offered_course_model
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

    ScreenShot {
        id: screenshot
        callback: function(){
            exclude_item.visible = true
            popup_notifier.text = "تصویر با موفقیت ذخیره شد!"
            popup_notifier.show()
        }
    }

    Popup {
        id: schedule_popup
        modal: true
        width: parent.width
        height: parent.height / 1.45
        anchors.centerIn: Overlay.overlay
        background: Rectangle { color: "transparent" }
        Notifier {
            id: popup_notifier
            showType: Notifier.ShowType.LeftToRight
            anchors.top: parent.top
            anchors.topMargin: 50
            z: 2
            font.family: regular_font.name
        }
        Rectangle {
            id: popup_contents
            width: parent.width
            height: parent.height
            color: "transparent"
            Rectangle {
                id: extra_rec
                anchors.left: parent.left
                anchors.leftMargin: 3
                width: 100
                height: 35
                color: "#1D2025"
                radius: 5
                Icon {
                    id: schedule_save_icon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    text: "\u1f4b" // save icon
                    color: "#FFFFFF"
                    description: "ذخیره برنامه هفتگی"
                    clickAble: true
                    onClicked: screenshot.saveItem(popup_contents, schedule_save_icon)
                }

                Label {
                    id: schedule_weight_lable
                    color: "white"
                    text: qsTr("%1 واحد").arg(Number(table_view.weightOfChoosed).toLocaleString(Qt.locale("fa_IR"), "f", 0))
                    font.pixelSize: 14
                    font.family: regular_font.name
                    anchors.centerIn: parent
                }
            }

            ScheduleTable {
                id: schedule_table
                anchors.top: extra_rec.bottom
                width: parent.width
                height: parent.height - extra_rec.height
                hasWarning: true
                model: offered_course_handler.restoreSchedule()
            }
        }

        MyButton {
            id: clear_schedule_btn
            anchors.top: popup_contents.bottom
            anchors.topMargin: 5
            anchors.right: popup_contents.right
            bgColor: "#19B99A"
            radius: 8
            font.family: regular_font.name
            width: 150
            height: 50
            text: "یادت نگهدار"
            iconText: "\ue80a" // bookmark icon
            iconSize: 17
            onClicked: {
                Settings.setValue("offeredSchedule", schedule_table.serialize())
                popup_notifier.text = "برنامه هفتگی برای استفاده های بعدی ذخیره شد!"
                popup_notifier.show()
            }
        }
        MyButton {
            id: save_schedule_btn
            anchors.top: clear_schedule_btn.top
            anchors.right: clear_schedule_btn.left
            anchors.rightMargin: 5
            bgColor: "#e54304"
            radius: 8
            font.family: regular_font.name
            width: 150
            height: 50
            iconText: "\ue808" // trash icon
            iconSize: 17
            text: "پاک کن"
            onClicked: {
                schedule_table.clear()
                table_view.clearAllChoosed()
            }
        }
    }

    MyButton {
        id: constructed_schedule_btn
        anchors.right: table_view.right
//        anchors.rightMargin: 10
        y: 20
        width: 170
        height: 50
        // TODO: change text to a more meaningful text
        text: "برنامه ساخته شده"
        bgColor: "#19B99A"
        radius: 8
        font.family: regular_font.name
        onClicked: schedule_popup.open()
    }

    MyTableView {
        id: table_view
        property int weightOfChoosed: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: constructed_schedule_btn.bottom
        anchors.topMargin: 10
        width: parent.width - 30
        height: parent.height - constructed_schedule_btn.height - 50
        model: 0
        columnItem: tableview_column

        onInitialChoose: weightOfChoosed += offered_course_model.getCourseWeight(index)
        onChoosedCleared: weightOfChoosed = 0

        onChoosed: {
            var obj = offered_course_model.toScheduleFormat(index);
            var collision_result = schedule_table.checkCollision(obj);

            if (collision_result[0] === ScheduleTable.NoCollision || collision_result[0] === ScheduleTable.ExamWarning) {
                if (collision_result[0] === ScheduleTable.ExamWarning) {
                    obj.warningForCourses = collision_result[1]
                }
                schedule_table.addElement(obj)
                weightOfChoosed += offered_course_model.getCourseWeight(index)
                return;
            }
            undoChoose(index)
            notifier.text = qsTr("%1 درس مورد نظر با درس %2 تداخل دارد!").arg(collision_result[0] === ScheduleTable.TimeCollision ? "زمان" : "امتحان").arg(collision_result[1])
            notifier.show()
        }

        onUnchoosed: {
            schedule_table.removeElement(offered_course_model.toScheduleFormat(index))
            weightOfChoosed -= offered_course_model.getCourseWeight(index)
        }
    }

    Component {
        id: tableview_column
        MyTableView.BaseColumnItem {
            Column {
                anchors.centerIn: parent
                width: parent.width - 5
                Label {
                    width: parent.width
                    horizontalAlignment: Label.AlignHCenter
                    wrapMode: Label.WordWrap
                    font.family: regular_font.name
                    text: model[role]
                    color: model.isChoosed ? "#757575" : "#FFFFFF"
                }
                Icon {
                    visible: role == "capacity" && model.sex !== OfferedCourseModel.None
                    width: parent.width
                    text: model.sex
                    horizontalAlignment: Label.AlignHCenter
                    color: model.isChoosed ? "#757575" : "#FFFFFF"
                }
            }
        }
    }
}

