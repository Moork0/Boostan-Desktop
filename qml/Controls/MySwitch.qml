import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property alias checked: control.checked
    width: 160
    height: 55

    Switch {
        id: control

        indicator: Rectangle {
            id: control_bg
            implicitWidth: 130
            implicitHeight: 64
            width: root.width - (check_indicator.width * 2)
            height: root.height
            radius: 48
            color: "#f6f6f6"
            border.color: "#ececec"
            border.width: 3

            Rectangle {
                id: switch_circle
                x: control.checked ? parent.width - width - 10 : 10
                width: root.height * 0.8
                height: width
                radius: width
                anchors.verticalCenter: parent.verticalCenter
                color: control.checked ? "#5ef499" : "#cccccc"
                border.color: control.checked ? "#86e2ac" : "#d1d1d1"

                Behavior on x {
                    enabled: true
                    NumberAnimation {
                        duration: 200 / 3
                        easing.type: Easing.OutInBounce
                    }
                }

                Behavior on color { ColorAnimation { duration: 200} }
            }
        }

    }

    Rectangle {
        id: check_indicator
        width: 16
        height: 16
        radius: height
        color: control.checked ? "#5ef499" : "#d96763"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: control.right
        anchors.leftMargin: -15
    }

}
