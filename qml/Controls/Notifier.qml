/*
    * A notifier component which used for send temporary messages to user
*/

import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root
    opacity: 0.0
    visible: false
    width: Math.max(notifier_text.contentWidth, notifier_solution.contentWidth) + 20
    height: notifier_text.contentHeight + notifier_solution.contentHeight + 20

    // identify the animation direction of notifier
    // Do not use anchors on direction of showType
    property int showType: Notifier.ShowType.DownToUp
    property alias bgColor: notifier_background.color
    property alias textColor: notifier_text.color
    property alias text: notifier_text.text
    property alias solution: notifier_solution.text
    property alias font: notifier_text.font
    property real distance: showType === Notifier.ShowType.DownToUp ? root.parent.height + root.height + 20 : -root.width
    enum ShowType {
        DownToUp,
        LeftToRight
    }

    function show() {
        root.state = "not-visible"
        root.state = "visible"
    }

    Rectangle {
        id: notifier_background
        anchors.fill: parent
        color: "#19B99A"
        radius: 5
        Column {
            anchors.centerIn: notifier_background
            Label {
                id: notifier_text
                anchors.horizontalCenter: parent.horizontalCenter
                text: "این یک متن ارور است که باید ببینید"
                color: "#FFFFFF"
                font.pixelSize: 15
                font.weight: Font.DemiBold
            }
            Label {
                id: notifier_solution
                visible: text !== ""
                anchors.horizontalCenter: parent.horizontalCenter
//                text: "این یک متن راهنماست"
                text: ""
                color: notifier_text.color
                font.family: notifier_text.font.family
                font.pixelSize: 14
                font.weight: Font.DemiBold
            }
        }


    }

    states: [
        State {
            name: "visible"
            PropertyChanges {
                target: root
                visible: true
                opacity: 1.0
                y: showType === Notifier.ShowType.DownToUp ? root.parent.height - root.height - 20 : root.y
                x: showType === Notifier.ShowType.LeftToRight ? 20 : root.x
            }
            // start fading out slowly
            PropertyChanges {
                target: fade_animation
                running: true
            }
        },
        State {
            name: "not-visible"
            PropertyChanges {
                target: root
                opacity: 0.0
                visible: false
            }
        }
    ]

    transitions: [
        Transition {
            from: "not-visible"
            NumberAnimation {
                target: root
                property: "opacity"
                duration: 200
                from: 0.0
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: root
                property: showType === Notifier.ShowType.DownToUp ? "y" : "x"
                from: distance
                duration: 200
                easing.type: Easing.OutElastic
            }
        }
    ]


    SequentialAnimation {
        id: fade_animation
        NumberAnimation {
            target: root
            property: "opacity"
            duration: 3200
            from: 1.0
            to: 0.0
            easing.type: Easing.InOutQuad
        }
        PropertyAnimation {
            target: root
            property: "visible"
            to: false
            duration: 100 // no effect
        }
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        // stop fading when mouse got into area
        onEntered: {
            fade_animation.stop()
            root.opacity = 1.0
        }
        onExited: fade_animation.start()
        onClicked: root.state = "not-visible"
    }
}

