import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"
import "../Helpers"
import API.OfferedCourseHandler 1.0

Page {
    id: offered_course_page

    OfferedCourseHandler {
        id: offered_course_handler
        Component.onCompleted: start()
        onFinished: {
            sendDataTo(offered_course_model)
            listview.model = offered_course_model
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

    MyButton {
        id: constructed_schedule_btn
        anchors.right: listview.right
//        anchors.rightMargin: 10
        y: 20
        width: 170
        height: 50
        // TODO: change text to a more meaningful text
        text: "برنامه ساخته شده"
        bgColor: "#19B99A"
        radius: 8
        font.family: regular_font.name
    }

    MyTableView {
        id: listview
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: constructed_schedule_btn.bottom
        anchors.topMargin: 10
        width: parent.width - 30
        height: parent.height - constructed_schedule_btn.height - 50
        model: 0
    }
}

