import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Rectangle {
    Layout.alignment: Qt.AlignRight
    color: "transparent"
    Layout.fillHeight: true
    Layout.fillWidth: true
    ToolTip.visible: course_area.containsMouse && modelData.name != ""
    ToolTip.delay: 500
    ToolTip.text: "استاد: " + modelData.teacher + "<br>تاریخ امتحان:‌ " + modelData.exam
    Label {
        width: parent.width - 5
        anchors.centerIn: parent
        font.family: "Sahel"
        color: "#FFFFFF"
        text: modelData.name
        wrapMode: Label.WordWrap
        horizontalAlignment: Label.AlignHCenter
    }
    MouseArea {
        id: course_area
        anchors.fill: parent
        hoverEnabled: true
    }

    Rectangle {
        y: 0
        width: parent.width + 4
        height: 2
        color: "#262A2F"
    }
    Rectangle {
        width: 2
        height: parent.height + 4
        x: parent.width
        color: "#262A2F"
    }
}
