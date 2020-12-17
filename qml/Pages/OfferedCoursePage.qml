import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"
import "../Helpers"

Page {
    id: offered_course_page
    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#262A2F"
    }

    MyButton {
        id: constructed_schedule_btn
        anchors.right: container_level1.right
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

    // header for the listview
    Rectangle {
        id: container_level1
        color: "#19B99A"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: constructed_schedule_btn.bottom
        anchors.topMargin: 10
        width: parent.width - 30
        height: parent.height - constructed_schedule_btn.height - 50
        radius: 10
        RowLayout {
            id: table_header_layout
            width: parent.width - 10
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            layoutDirection: Qt.RightToLeft
            spacing: 0
            Repeater {
                id: table_header_repeater
                model: ["گروه", "شماره درس", "نام درس", "واحد", "ظرفیت", "استاد", "زمان", "محل برگزاری", "امتحان"]
                property var sizes: [0.05, 0.11, 0.18, 0.04, 0.06, 0.13, 0.16, 0.11, 0.1]
                Item {
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: table_header_layout.width * table_header_repeater.sizes[index]
//                    border.color: "red"
//                    border.width: 1
//                    color: "transparent"
                    Layout.alignment: Qt.AlignVCenter
                    Label {
                        anchors.centerIn: parent
                        font.family: regular_font.name
                        font.bold: true
                        text: modelData
                        color: "#000000"
                    }
                    Rectangle {
                        visible: index !== 0
                        anchors.right: parent.right
                        height: 60 // summation of container_level2 and listview_bg margins from the container_level1
                        width: 2
                        color: "#262A2F"
                    }
                }
            }
        }
    }

    Rectangle {
        id: container_level2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: container_level1.top
        anchors.topMargin: 50
        width: container_level1.width
        height: container_level1.height - 50
        radius: 8
        color: "#262A2F"
        opacity: 0.7
    }

    Rectangle {
        id: listview_bg
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: container_level2.top
        anchors.topMargin: 10
        width: container_level2.width
        height: container_level2.height - 10
        radius: 8
        color: "#262A2F"
        opacity: 0.5
    }
    ListView {
        id: listview
        anchors.fill: listview_bg
        property var choosed: []
        clip: true
//            model: [
//                [{
//                    group: "76",
//                    courseNo: "5356535142",
//                    courseName: "مبانی ماتریس ها و جبرخطی",
//                    weight: 3,
//                    capacity: 50,
//                    sex: 1,
//                    teacher: "تقوی جلودار علی",
//                    time: "چهارشنبه ۸:۰۰ - ۱۰:۰۰ \n دوشنبه ۸:۰۰ - ۱۰:۰۰",
//                    place: "دانشکده ریاضی [409] \n [409]",
//                    exam: "99/7/16 \n 8:00 - 10:00"
//                }],
//                [{
//                    group: "76",
//                    courseNo: "5356535142",
//                    courseName: "مبانی ماتریس ها و جبرخطی",
//                    weight: 3,
//                    capacity: 50,
//                    sex: 1,
//                    teacher: "تقوی جلودار علی",
//                    time: "چهارشنبه ۸:۰۰ - ۱۰:۰۰ \n دوشنبه ۸:۰۰ - ۱۰:۰۰",
//                    place: "دانشکده ریاضی [409] \n [409]",
//                    exam: "99/7/16 \n 8:00 - 10:00"
//                }]
//            ]

        model: lmodel
        delegate: OfferedCourseDelegate { }
    }


    ListModel {
        id: lmodel

        ListElement {
            group: "76"
            courseNo: "5356535142"
            courseName: "مبانی ماتریس ها و جبرخطی"
            weight: 3
            capacity: 50
            sex: 1
            teacher: "تقوی جلودار علی"
            time: "چهارشنبه ۸:۰۰ - ۱۰:۰۰ \n دوشنبه ۸:۰۰ - ۱۰:۰۰"
            place: "دانشکده ریاضی [409] \n [409]"
            exam: "99/10/16 \n 8:00 - 10:00"
        }
        ListElement {
            group: "76"
            courseNo: "5356535142"
            courseName: "مبانی ماتریس ها و جبرخطی"
            weight: 3
            capacity: 50
            sex: 1
            teacher: "تقوی جلودار علی"
            time: "چهارشنبه ۸:۰۰ - ۱۰:۰۰ \n دوشنبه ۸:۰۰ - ۱۰:۰۰"
            place: "دانشکده ریاضی [409] \n [409]"
            exam: "99/10/16 \n 8:00 - 10:00"
        }
        ListElement {
            group: "76"
            courseNo: "5356535142"
            courseName: "مبانی ماتریس ها و جبرخطی"
            weight: 3
            capacity: 50
            sex: 1
            teacher: "تقوی جلودار علی"
            time: "چهارشنبه ۸:۰۰ - ۱۰:۰۰ \n دوشنبه ۸:۰۰ - ۱۰:۰۰"
            place: "دانشکده ریاضی [409] \n [409]"
            exam: "99/10/16 \n 8:00 - 10:00"
        }


    }
}

