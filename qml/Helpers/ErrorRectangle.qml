/*
    * Just a rectangle to show message about the unavailablity of data.
*/

import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root

    property string name: ""

    color: "#1D2025"
    z: 1
    Label {
        width: parent.width
        height: parent.height
        wrapMode: Label.WordWrap
        font.family: regular_font.name
        horizontalAlignment: Label.AlignHCenter
        verticalAlignment: Label.AlignVCenter
        text: qsTr("متاسفانه اطلاعات %1 موجود نیست! :(").arg(root.name)
        color: "white"
    }
}
