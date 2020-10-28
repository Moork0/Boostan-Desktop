import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root
    opacity: 0.0
    visible: false
    width: notifier_text.contentWidth + 20
    height: notifier_text.contentHeight + 20
    property int showType: Notifier.ShowType.DownToUp
    property alias bgColor: notifier_background.color
    property alias textColor: notifier_text.color
    property alias text: notifier_text.text
    property alias font: notifier_text.font
    property real distance: showType === Notifier.ShowType.DownToUp ? page_background.height + root.height + 20 : -root.width
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
        Label {
            id: notifier_text
            anchors.centerIn: notifier_background
            text: "این یک متن ارور است که باید ببینید"
            color: "#FFFFFF"
            font.family: "Medium"
            font.pixelSize: 15
            font.weight: Font.DemiBold
        }
    }

    states: [
        State {
            name: "visible"
            PropertyChanges {
                target: root
                visible: true
                opacity: 1.0
                y: showType === Notifier.ShowType.DownToUp ? page_background.height - root.height - 20 : root.y
                x: showType === Notifier.ShowType.LeftToRight ? 20 : root.x
            }
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
                easing.type: Easing.InOutQuad
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
        onEntered: {
            fade_animation.stop()
            root.opacity = 1.0
        }
        onExited: fade_animation.start()
        onClicked: root.state = "not-visible"
    }
}

