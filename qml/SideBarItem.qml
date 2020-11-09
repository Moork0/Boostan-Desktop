/*
    * A side bar item implementation
*/

import QtQuick 2.15

Item {
    id: root
    property alias isEnable: control.enabled
    signal clicked
    Rectangle {
        id: control
        anchors.fill: parent
        enabled: false
        color: "transparent"

    }

    // show a shadow when a item is in selected mode or has a mouse hovered on
    Rectangle {
        id: shadow
        width: root.width
        height: root.height
        anchors.right: root.right
        color: "#F8F7F2"
        opacity: control.enabled || mouse_area.containsMouse ? 0.08 : 0.0
        Behavior on opacity {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }

        NumberAnimation {
            id: click_anim
            target: shadow
            property: "width"
            from: 0
            to: root.width
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }

    // indicator that determine a item is in selected mode
    Rectangle {
        id: enable_indicator
        anchors.left: control.left
        visible: control.enabled
        width: 3
        height: control.height
        color: "#19B99A"
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            if (!control.enabled) {
                click_anim.start()
                root.clicked()
            }
        }
    }
}

