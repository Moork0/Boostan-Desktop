/*
    * Custom button component
*/

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Button {
    id: root
    text: "Button"
    font.capitalization: Font.MixedCase
    Keys.onReturnPressed: clicked()
    Keys.onEnterPressed: clicked()
    Keys.onSpacePressed: {}

    property var bgColor: "#3fc1c9"
    property alias border: __background.border
    property alias color: __text.color
    property alias radius: __background.radius
    property alias iconText: __icon.text
    property alias iconSize: __icon.font.pixelSize
    property alias area: __mousearea
    property alias direction: __content_layout.layoutDirection

    signal clicked()
    signal pressAndHold()

    contentItem: Item {
        RowLayout {
            id: __content_layout
            anchors.centerIn: parent
            opacity: __background.opacity

            Text {
                id: __icon
                visible: text == "" ? false : true
                Layout.bottomMargin: 3
                font.family: "fontello"
                color: __text.color
            }

            Text {
                id: __text
                text: root.text
                font: root.font
                color: "black"
            }
        }
    }

    background: Rectangle {
        id: __background
        property real mouse_opacity: 1.0
        implicitHeight: 30
        implicitWidth: 50
        radius: 3
        color: root.bgColor
        opacity: {
            if (!root.enabled) return 0.4;
            if (__mousearea.containsPress) return 0.65;
            return 1.0;
        }


        MouseArea {
            id: __mousearea
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                __background_presshold.x = __mousearea.mouseX
                __background.state = "PressedAndHold"
            }
            onReleased: {
                __background.state = ""
                root.clicked()
            }
        }

        Rectangle {
            /*
                * this rectangle will fill the whole button with animation when a click occur
            */
            id: __background_presshold
            height: __background.height
            width: 0
//            height: 0
            color: root.bgColor
            opacity: 0.5
            radius: __background.radius

        }

        Rectangle {
            /* shadow will be visible when mouse hovered on button or a click occured */
            id: __background_shadow
            anchors.fill: __background
            radius: __background.radius
            color: "#616161"
            opacity: (__mousearea.containsMouse || root.focus) && !__mousearea.containsPress ? 0.2 : 0.0
            Behavior on opacity {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
        }

        states: [
            State {
                name: "PressedAndHold"
                AnchorChanges {
                    target: __background_presshold
                    anchors.right: __background.right
                    anchors.left: __background.left
                }
            }
        ]

        transitions: [
            Transition {
                to: "PressedAndHold"
                SequentialAnimation {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                    AnchorAnimation {
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        ]

    }
}
