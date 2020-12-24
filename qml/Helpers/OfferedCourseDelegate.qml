import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

/**
    * this code make me sick. i made a 10 piece of code and each are almost equal to eachother.
    * I should use Repeater for this. but not now. i wanna get destroyed at refactor time.
**/

// TODO: make group and course number select and copyable.

Item {
    id: root
    height: 70
    width: listview.width
    property bool isChoosed: false
    property color textColor: isChoosed ? "#757575" : "#FFFFFF"
    property string fontFamily: regular_font.name

    RowLayout {
        width: parent.width - 10
        height: parent.height - 10
        layoutDirection: Qt.RightToLeft
        spacing: 0

        Item {
            id: group
            Layout.preferredWidth: parent.width * 0.05
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
//            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: group_text
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                text: model["group"]
                color: root.textColor
            }
            Rectangle {
//                visible: index !== 0
                visible: false
                anchors.right: parent.right
                height: 60 // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: course_no
            Layout.preferredWidth: table_header_layout.width * 0.11
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
//            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: course_no_text
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                text: model.courseNumber
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: course_name
            Layout.preferredWidth: table_header_layout.width * 0.18
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: course_name_text
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                text: model.courseName
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: weight
            Layout.preferredWidth: table_header_layout.width * 0.04
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: weight_text
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                text: model.weight
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: capacity
            Layout.preferredWidth: table_header_layout.width * 0.06
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: capacity_text
                anchors.centerIn: parent
                                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                text: model.capacity
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: teacher
            Layout.preferredWidth: table_header_layout.width * 0.13
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: teacher_text
                anchors.centerIn: parent
                                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                text: model.teacher
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: time
            Layout.preferredWidth: table_header_layout.width * 0.16
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: time_text
                anchors.centerIn: parent
                                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily

                text: model.time
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: place
            Layout.preferredWidth: table_header_layout.width * 0.11
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: place_text
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                font.pixelSize: 12
                text: model.place
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

        Item {
            id: exam
            Layout.preferredWidth: table_header_layout.width * 0.1
            Layout.preferredHeight: parent.height
//            border.color: "red"
//            border.width: 1
            // color: "transparent"
            Layout.alignment: Qt.AlignVCenter
            Label {
                id: exam_text
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: root.fontFamily
                font.pixelSize: 13
                text: model.exam
                color: root.textColor
            }
            Rectangle {
                anchors.left: parent.right
                anchors.leftMargin: 3
                height: root.height // summation of container_level2 and listview_bg margins from the container_level1
                width: 2
                color: "#262A2F"
            }
        }

    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        onPressAndHold: {
            if (isChoosed) {
                var ind = listview.choosed.indexOf(model.index)
                listview.choosed.splice(ind, 1)
            }else {
                listview.choosed.push(index);
            }
            console.log(listview.choosed)
            isChoosed = !isChoosed
        }
    }

    Rectangle {
        visible: index !== listview.count
        anchors.bottom: parent.bottom
        width: listview.width
        height: 4
        color: "#19B99A"
        opacity: 0.6
    }
}

