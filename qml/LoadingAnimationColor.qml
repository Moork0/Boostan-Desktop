import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: "#1D2025"
    visible: true

    SequentialAnimation on color {
        id: animation
        running: root.visible
        loops: Animation.Infinite
        ColorAnimation {
            from: "#1D2025"
            to: "#424242"
            duration: 2000
        }
        ColorAnimation {
            from: "#424242"
            to: "#1D2025"
            duration: 2000
        }
    }
}
