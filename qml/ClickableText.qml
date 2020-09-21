import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    signal clicked()
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: parent.clicked()
    }
}
