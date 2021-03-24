import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property string name: ""

    anchors.fill: parent
    color: "#1D2025"
    z: 1
    Label {
        anchors.centerIn: parent
        font.family: regular_font.name
        text: qsTr("متاسفانه اطلاعات %1 موجود نیست! :(").arg(root.name)
        color: "white"
    }
}
