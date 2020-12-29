import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

Item {
    id: root
    required property var model
    property alias headerTitles: hours_repeater.model
    property alias sideTitles: days_repeater.model
    property alias headerDescription: table_desc.text

    function setEelement(row, column, object)
    {
        var len = row.length
        for (var i = 0; i < len; ++i) {
            day_row.itemAt(row[i]).visibleChildren[1]["element_rep"].itemAt(column[i])["dataModel"] = object
        }
    }

    function clearEelement(row, column)
    {
        var len = row.length
        for (var i = 0; i < len; ++i) {
            day_row.itemAt(row[i]).visibleChildren[1]["element_rep"].itemAt(column[i])["dataModel"] = null
        }
    }

    Component.onCompleted: {
        var len = root.model.length
        for (var i = 0; i < len; ++i) {
            setEelement(root.model[i].row, root.model[i].column, root.model[i])
        }
    }

    Rectangle {
        id: table_schedule_bg
        anchors.fill: parent
        color: "#1D2025"
        radius: 10
    }

    // this is the first cell (from top-right) of the table.
    Item {
        id: blank_space
        anchors.right: parent.right
        anchors.top: parent.top
        width: 90
        height: 40
        Label {
            id: table_desc
            anchors.centerIn: parent
            text: "روز / ساعت"
            font.family: regular_font.name
            color: "#FFFFFF"
        }
    }

    RowLayout {
        id: hours
//        y: -2
        layoutDirection: Qt.RightToLeft
        width: parent.width - blank_space.width
        height: blank_space.height
        spacing: 0
        Repeater {
            id: hours_repeater
            model: ["۸-۱۰", "۱۰-۱۲", "۱۳-۱۵", "۱۵-۱۷", "۱۷-۱۹"]
            Rectangle {
                Layout.alignment: Qt.AlignRight
                color: "transparent"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Label {
                    anchors.centerIn: parent
                    font.family: regular_font.name
                    color: "#FFFFFF"
                    text: modelData
                }
                Rectangle {
                    x: parent.width
                    width: 2
                    height: root.height
                    color: "#262A2F"
                }
            }
        }
    }

    ColumnLayout {
        id: days
        anchors.right: parent.right
        anchors.top: blank_space.bottom
        anchors.topMargin: -4
        spacing: 0
        width: blank_space.width
        height: table_schedule_bg.height - blank_space.height + 4
        Repeater {
            id: days_repeater
            model: ["شنبه", "یکشنبه", "دوشنبه", "سه‌شنبه", "چهارشنبه"]
            Rectangle {
                Layout.alignment: Qt.AlignRight
                color: "transparent"
                Layout.fillHeight: true
                Layout.fillWidth: true
                Label {
                    anchors.centerIn: parent
                    font.family: regular_font.name
                    color: "#FFFFFF"
                    text: modelData
                }
            }
        }
    }

    ColumnLayout {
        id: courses
        anchors.top: hours.bottom
        width: parent.width - days.width
        height: parent.height - hours.height
        spacing: 0
        Repeater {
            id: day_row
            model: days_repeater.count
            Item {
                width: parent.width
                Layout.fillHeight: true
                Rectangle {
                    anchors.top: parent.top
                    color: "#262A2F"
                    width: root.width
                    height: 2
                }
                RowLayout {
                    width: parent.width
                    height: parent.height
                    layoutDirection: Qt.RightToLeft
                    spacing: 0
                    property var element_rep: element_repeater
                    Repeater {
                        id: element_repeater
                        // get the courses of a day 'index' as a array of objects
                        model: hours_repeater.count
                        delegate: table_element
                    }
                }
            }
        }
    }

    Component {
        id: table_element
        Rectangle {
            Layout.alignment: Qt.AlignRight
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true

            property var dataModel: null

            ToolTip.visible: course_area.containsMouse && dataModel != null
            ToolTip.delay: 500
            ToolTip.text: dataModel != null ? "استاد: " + dataModel.teacher + "<br>تاریخ امتحان:‌ " + dataModel.exam : ""

            Label {
                width: parent.width - 5
                anchors.centerIn: parent
                font.family: regular_font.name
                color: "#FFFFFF"
                text: dataModel == null ? "" : dataModel.name
                wrapMode: Label.WordWrap
                horizontalAlignment: Label.AlignHCenter
            }
            MouseArea {
                id: course_area
                anchors.fill: parent
                hoverEnabled: true
            }
        }
    }

}
