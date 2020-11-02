import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
//import QtCharts 2.15
import API.BriefInfoHandler 1.0

Page {
    id: dashboard_page

    BriefInfoHandler {
        id: dashboard_handler
        Component.onCompleted: {
            start()
        }
        onFinished: success ? console.log(getSemesterYears(), getSemesterAvgs()) : error_handler.raiseError(errorCode)
    }

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
//                ChartView {
//                    title: "Bar series"
//                    anchors.fill: parent
//                    legend.alignment: Qt.AlignBottom
//                    antialiasing: true

//                    BarSeries {
//                        id: mySeries
//                        axisX: BarCategoryAxis { categories: ["2007", "2008", "2009", "2010", "2011", "2012" ] }
//                        BarSet { label: "Bob"; values: [2, 2, 3, 4, 5, 6] }
//                        BarSet { label: "Susan"; values: [5, 1, 2, 4, 1, 7] }
//                        BarSet { label: "James"; values: [3, 5, 8, 13, 5, 8] }
//                    }
//                }
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
                        text: "نام و نام خانوادگی: " + dashboard_handler.briefInfo.name
                        wrapMode: Text.WordWrap
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "دوره آموزشی:‌ " + dashboard_handler.briefInfo.studyType
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "رشته تحصیلی: " + dashboard_handler.briefInfo.field
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        property var t
                        Component.onCompleted: {
                            t = Qt.locale("fa_IR")
                            t.NumberOption = Locale.OmitGroupSeparator
                            //console.log()
                        }

                        text: "عدد: " + dashboard_handler.briefInfo.id
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "واحد گذرانیده: " + dashboard_handler.briefInfo.passedUnits
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight
                        text: "معدل کل: " + dashboard_handler.briefInfo.average
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
