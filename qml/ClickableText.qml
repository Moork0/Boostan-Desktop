/*
    * This Control is a Label with support of MouseArea
*/

import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    signal clicked()
    property alias area: mouse_area
    MouseArea {
        id: mouse_area
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: parent.clicked()
    }
}
