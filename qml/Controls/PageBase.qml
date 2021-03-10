import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: root
    property bool __isActive: true
    property var __viewManUid

    signal __timedOut(var uid)

    on__IsActiveChanged: {
        if (__isActive === false) {
            __destruction_timer.restart()
        } else {
            __destruction_timer.stop()
        }
    }

    Timer {
        id: __destruction_timer
        interval: 15000
        onTriggered: root.__timedOut(root.__viewManUid)
    }
}
