/*
    * This Component is a color changing animation which is being used for loadings
*/

import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: "#1D2025"

    property color from: "#1D2025"
    property color to: "#424242"

    SequentialAnimation on color {
        id: animation
        running: root.visible
        loops: Animation.Infinite

        ColorAnimation {
            from: root.from
            to: root.to
            duration: 2000
        }

        ColorAnimation {
            from: root.to
            to: root.from
            duration: 2000
        }
    }
}
