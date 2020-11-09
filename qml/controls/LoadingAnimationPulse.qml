/*
    * This component is a animation consists of some rectangles that being short and tall
*/

import QtQuick 2.15

Item {
    id: root

    property alias barCount: repeater.model
    property color color: "white"
    property int spacing: 5
    property bool running: true

    Repeater {
        id: repeater
        delegate: Component {
            Rectangle {
                width: (root.width / root.barCount) - root.spacing
                height: root.height
                x: index * width + root.spacing * index
                transform: Scale {
                    id: rectScale
                    origin {
                        x: width / 2
                        y: height / 2
                    }
                }
                transformOrigin: Item.Center
                color: root.color

                SequentialAnimation {
                    id: anim
                    loops: Animation.Infinite
                    running: root.running
                    NumberAnimation { target: rectScale; property: "yScale"; from: 1; to: 1.5; duration: 300 }
                    NumberAnimation { target: rectScale; property: "yScale"; from: 1.5; to: 1; duration: 300 }
                    PauseAnimation { duration: root.barCount * 150 }
                }
            }
        }
    }
}
