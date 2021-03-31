/*
    * This is a Base for creating pages components which has integrated
    * with ViewManager component.
    * This component check the activeness of the page and will tell the ViewManager to destroy the page
    * after certain time of inactivity.
*/

import QtQuick 2.15
import QtQuick.Controls 2.15

Page {
    id: root

    // indicate if the Page should never destroy.
    property bool noDestruct: false
    property bool __isActive: true
    // unique id of the page in ViewManager
    property var __viewManUid

    // Notify the timer ending.
    signal __timedOut(var uid)

    on__IsActiveChanged: {
        if (__isActive === false && noDestruct === false) {
            __destruction_timer.restart()
        } else {
            __destruction_timer.stop()
        }
    }

    // Timer for measuring inactivity of page.
    Timer {
        id: __destruction_timer
        interval: 15000
        onTriggered: root.__timedOut(root.__viewManUid)
    }
}
