import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    /**  Properties  **/

    property alias model: listview.model
    property var columnKey: ["group", "courseNumber", "courseName", "weight", "capacity", "teacher", "time", "place", "exam"]
    property alias columnTitle: table_header_repeater.model
    property alias columnSizes: table_header_repeater.sizes
    property var columnItem: column_repeater_delegate
    property var choosedList: []

    /**  Signals  **/
    signal choosed(int index)
    signal unchoosed(int index)
    signal initialChoose(int index)
    signal choosedCleared()

    function undoChoose(index)
    {
        listview.itemAtIndex(index).rowData["isChoosed"] = false
        var ind = root.choosedList.indexOf(index)
        root.choosedList.splice(ind, 1)
    }

    function clearAllChoosed ()
    {
        for (var i = 0; i < choosedList.length; ++i) {
            var obj = listview.itemAtIndex(choosedList[i])
            obj.rowData["isChoosed"] = false
        }
        root.choosedList = []
        root.choosedCleared()
    }

    // header for the listview
    Rectangle {
        id: container_level1
        color: "#19B99A"
        width: root.width
        height: root.height
        radius: 10
        RowLayout {
            id: table_header_layout
            width: parent.width - 10
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            layoutDirection: Qt.RightToLeft
            spacing: 0
            Repeater {
                id: table_header_repeater                
                model: ["گروه", "شماره درس", "نام درس", "واحد", "ظرفیت", "استاد", "زمان", "محل برگزاری", "امتحان"]
                property var sizes: [0.05, 0.11, 0.18, 0.04, 0.06, 0.13, 0.16, 0.11, 0.1]
                Item {
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: table_header_layout.width * table_header_repeater.sizes[index]
                    Label {
                        anchors.centerIn: parent
                        font.family: regular_font.name
                        font.bold: true
                        text: modelData
                        color: "#000000"
                    }
                    Rectangle {
                        visible: index !== 0
                        anchors.right: parent.right
                        height: 60 // summation of container_level2 and listview_bg margins from the container_level1
                        width: 2
                        color: "#262A2F"
                    }
                }
            }
        }
    }

    Rectangle {
        id: container_level2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: container_level1.top
        anchors.topMargin: 50
        width: container_level1.width
        height: container_level1.height - 50
        radius: 8
        color: "#262A2F"
        opacity: 0.7
    }

    Rectangle {
        id: listview_bg
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: container_level2.top
        anchors.topMargin: 10
        width: container_level2.width
        height: container_level2.height - 10
        radius: 8
        color: "#262A2F"
        opacity: 0.5
    }
    ListView {
        id: listview
        anchors.fill: listview_bg
        property var choosed: []
        clip: true
        boundsBehavior: Flickable.OvershootBounds
        delegate: delegate
    }

    Component {
        id: delegate
        Item {
            id: delegate_root
            height: 70
            width: listview.width
            property var rowData: model
            Component.onCompleted: {
                rowData["isChoosed"] = rowData.isChoosed ?? false;
                if (rowData["isChoosed"]) {
                    choosedList.push(index)
                    initialChoose(index)
                }
            }

            RowLayout {
                width: parent.width - 10
                height: parent.height - 10
                anchors.horizontalCenter: parent.horizontalCenter
                layoutDirection: Qt.RightToLeft
                spacing: 0

                Repeater {
                    id: column_repeater
                    model: root.columnKey
                    property var rowIndex: index
                    Item {
                        id: column_repeater_root_delegate
                        Layout.preferredWidth: parent.width * table_header_repeater.sizes[index]
                        Layout.preferredHeight: parent.height
                        Layout.alignment: Qt.AlignVCenter
                        Component.onCompleted: {
                            root.columnItem.incubateObject(column_repeater_root_delegate, {
                                                             "model": delegate_root.rowData, "role": modelData,
                                                             "index": index
                                                         })
                        }
                        Rectangle {
                            visible: index !== 0
            //                visible: false
                            anchors.right: parent.right
                            height: 70 // summation of container_level2 and listview_bg margins from the container_level1
                            width: 2
                            color: "#262A2F"
                        }
                    }
                }
            }

            MouseArea {
                id: mouse_area
                anchors.fill: parent
                onPressAndHold: {
                    if (delegate_root.rowData.isChoosed) {
                        var ind = root.choosedList.indexOf(index)
                        root.choosedList.splice(ind, 1)
                        delegate_root.rowData.isChoosed = false
                        root.unchoosed(index)
                    }else {
                        root.choosedList.push(index);
                        delegate_root.rowData.isChoosed = true
                        root.choosed(index)
                    }
                }
            }

            Rectangle {
                visible: index !== listview.count - 1
                anchors.bottom: parent.bottom
                width: listview.width
                height: 4
                color: "#19B99A"
                opacity: 0.6
            }
        }

    }

    component BaseColumnItem: Item {
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        required property var model
        required property var role
        required property int index
    }

    Component {
        id: column_repeater_delegate
        BaseColumnItem {
            Label {
                anchors.centerIn: parent
                width: parent.width - 5
                horizontalAlignment: Label.AlignHCenter
                wrapMode: Label.WordWrap
                font.family: regular_font.name
                text: model[role]
                color: model.isChoosed ? "#757575" : "#FFFFFF"
            }
        }
    }
}
