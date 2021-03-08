import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    property bool __isActive: true
    property int __viewManagerIndex
    property var __viewManager
    on__IsActiveChanged: {
        if (isActive === false) {
            __destruction_timer.restart()
        } else {
            __destruction_timer.stop()
        }
    }

    Timer {
        id: __destruction_timer
        interval: 15000
        onTriggered: __viewManager.remove(__viewManagerIndex)
    }
}
