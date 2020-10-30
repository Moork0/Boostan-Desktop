import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/*
 *  TODO: make animation for errors (possibly with path's)
 */
Item {
    id: root
    property alias direction: row_layout.layoutDirection
    property alias icon: icon.text
    property alias iconSize: icon.font.pixelSize
    property alias text: tfield_root.text
    property alias placeHolder: tfield_root.placeholderText
    property alias mode: tfield_root.echoMode
    property alias horizontalAlignment: tfield_root.horizontalAlignment
    property bool isEmpty: tfield_root.text.trim() == ""

    signal focusChanged

    Rectangle {
        id: input_container
        width: root.width
        height: root.height
        color: "#FFFFFF"
        radius: 8
        border.width: 1
        border.color: "transparent"


        RowLayout {
            id: row_layout
            width: parent.width - 10
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 2

            Label {
                id: icon
                visible: text == "" ? false : true
                Layout.rightMargin: 2
                font.family: "fontello"
                font.pixelSize: 30
            }

            Rectangle {
                id: seprator
                visible: icon.visible
                Layout.leftMargin: 6
                Layout.rightMargin: 6
                width: 1
                height: parent.height
                color: "#616161"
            }

            TextField {
                id: tfield_root
                placeholderText: "Here text"
                Layout.fillWidth: true
                Layout.fillHeight: true
                bottomPadding: tfield_bg.implicitHeight / 5
                selectByMouse: true
                onFocusChanged: {
                    root.focusChanged()
                    if (tfield_root.focus) {
                        input_container.border.color = "#2196F3"
                        return;
                    }

                    if (!tfield_root.focus && tfield_root.text.trim() == "") {
                        input_container.border.color = "#E65100"
                        return;
                    }
                    input_container.border.color = "transparent"
                }

                background: Rectangle {
                    id: tfield_bg
                    implicitHeight: 40
                    color: "transparent"
                }


                cursorDelegate: Rectangle {
                    id: tfield_cursor
                    visible: tfield_root.focus
                    height: tfield_root.height - 20
                    width: 1
                    color: "#212121"
                    anchors.verticalCenter: tfield_root.verticalCenter

                    SequentialAnimation {
                        running: visible
                        loops: Animation.Infinite
                        NumberAnimation {
                            target: tfield_cursor
                            property: "opacity"
                            duration: 100
                            to: 1
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            target: tfield_cursor
                            property: "opacity"
                            duration: 100
                            to: 0
                            easing.type: Easing.InOutQuad
                        }

                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutQuad
                        }
                    }
                }
            }
        }
    }

}
