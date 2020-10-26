import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: dashboard_page
    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#262A2F"
    }

    ColumnLayout {
        id: layout
        width: parent.width - 20
        height: parent.height - 20
        anchors.centerIn: parent
        spacing: 10
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.width / 2
            spacing: 10

            Rectangle {
                Layout.preferredWidth: parent.width * 0.70
                Layout.fillHeight: true
                color: "#1D2025"
                radius: 10
            }

            Rectangle {
                id: personal_info_bg
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#1D2025"
                radius: 10
                ColumnLayout {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 25
                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredWidth: personal_info_bg.width
                        text: "نام و نام خانوادگی: سیده سوزان محسنی سنگتابی"
                        wrapMode: Text.WordWrap
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "دوره آموزشی:‌ روزانه"
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "رشته تحصیلی: مراقبت های ویژه حیوانات"
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "عدد: " + Number(123456789123).toLocaleString(Qt.locale("fa_IR"), "f", 0)
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "شماره ملی: 55457897789987987"
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "معدل کل: 20"
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }

                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height / 2
            spacing: 10

            Rectangle {
                id: table_schedule
                Layout.fillWidth: true                
                Layout.fillHeight: true
                width: layout.width
                height: layout.height / 2
                color: "#1D2025"
                radius: 10
                GridLayout {
                    id: days_column
                    anchors.right: table_schedule.right
                    anchors.top: table_schedule.top
                    anchors.topMargin: -2
//                    anchors.topMargin: 50
                    width: table_schedule.width * 0.1
                    height: table_schedule.height
                    columns: 1
//                    rows: 6
                    rowSpacing: 0
                    layoutDirection: Qt.RightToLeft
                    Repeater {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignRight
//                        model: ["روز / ساعت", "شنبه", "یکشنبه", "دوشنبه", "سه شنبه", "چهارشنبه"]
                        model: ListModel {
                            ListElement {
                                name:"hhh"
                            }
                        }

                        delegate: Rectangle {
                            Layout.alignment: Qt.AlignRight
                            border.width: 2
                            border.color: "#262A2F"
                            color: "transparent"
                            width: days_column.width
                            Layout.fillHeight: true
                            Label {
                                anchors.centerIn: parent
                                font.family: "Sahel"
                                color: "#FFFFFF"
                                text: model.name
                            }
                            Rectangle {
                                anchors.bottom: parent.bottom
                                width: parent.width - 5
                                height: 2
                                x: 2
                                color: "#1D2025"
                            }
                        }
                    }
                }

            }
        }
    }
}
