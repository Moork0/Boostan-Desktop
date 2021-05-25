import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"


PageBase {
    id: settings_page

    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#262A2F"
    }


    Rectangle {
        id: tabbar_bg
        anchors.horizontalCenter: parent.horizontalCenter
        y: 100
        width: 400
        height: 35
        color: "#33363A"
        radius: 20
        RowLayout {
            width: parent.width
            height: parent.height
            spacing: 0
            Repeater {
                id: tabbar_repeater
                model: ["تنظیمات گلستان", "تنظیمات بوستان"]
                delegate: tabbar_comp
                signal tabChanged(var index)
                onTabChanged: {
                    console.log("index ", index)
                }
            }

        }
    }

    Component {
        id: tabbar_comp
        Rectangle {
            id: tabbar_comp_root
            property bool selected: false
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "transparent"
            radius: tabbar_bg.radius
            Behavior on color {
                ColorAnimation {
                    duration: 200
                }
            }

            Label {
                anchors.centerIn: parent
                text: modelData
                color: "#FCFCFC"
                font.family: regular_font.name
                font.weight: Font.Medium
            }

            MouseArea {
                id: m_area1
                anchors.fill: parent
                onClicked: {
                    if (tabbar_comp_root.selected)
                        return;
                    tabbar_comp_root.selected = true
                    tabbar_repeater.tabChanged(index)
                }
            }

            onSelectedChanged: {
                color = selected ? "#159E84" : "transparent"
            }

            Connections {
                target: tabbar_repeater
                function onTabChanged(ind)
                {
                    if (ind !== index) {
                        tabbar_comp_root.selected = false
                    }
                }
            }
        }
    }
}
