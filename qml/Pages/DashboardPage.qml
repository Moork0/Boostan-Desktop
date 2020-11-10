import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.2
import API.BriefInfoHandler 1.0
import API.CourseScheduleHandler 1.0
import "../Controls"

Page {
    id: dashboard_page

    BriefInfoHandler {
        id: dashboard_handler
        Component.onCompleted: {
            start()
        }
        onFinished: success ? schedule_handler.start(currentYear) : error_handler.raiseError(errorCode)
    }

    CourseScheduleHandler {
        id: schedule_handler
        onFinished: success ? console.log(" course success") : error_handler.raiseError(errorCode)
    }

    Rectangle {
        id: page_background
        anchors.fill: parent
        color: "#262A2F"
    }

    Notifier {
        id: notifier
        showType: Notifier.ShowType.LeftToRight
        anchors.top: parent.top
        anchors.topMargin: 50
        z: 2
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
                id: averages_plot
                Layout.preferredWidth: parent.width * 0.70
                Layout.fillHeight: true
                color: "#1D2025"
                radius: 10
                property bool ready: dashboard_handler.finished && dashboard_handler.success

                Icon {
                    id: save_plot
                    visible: averages_plot.ready
                    anchors { top: parent.top; left: parent.left; leftMargin: 10; topMargin: 10 }
                    text: "\u1f4b"
                    color: "#FFFFFF"
                    description: "ذخیره نمودار معدل"
                    clickAble: true
                    onClicked: file_dialog.saveItem(averages_plot, save_plot)
                }

                Plot {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height - 40
                    xAxis: averages_plot.ready ? dashboard_handler.getSemesterYears() : []
                    yAxis: averages_plot.ready && dashboard_handler.success ? dashboard_handler.getSemesterAvgs() : []
                }

                LoadingAnimationColor {
                    anchors.fill: parent
                    radius: 10
                    visible: !averages_plot.ready
                }

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
                        text: "نام و نام خانوادگی: " + right_pane.studentName
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

                        text: "شماره دانشجویی: " + dashboard_handler.briefInfo.id
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
                LoadingAnimationColor {
                    anchors.fill: parent
                    radius: 10
                    visible: !dashboard_handler.success
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height / 2
//            spacing: 10
            spacing: 0

            Rectangle {
                id: table_schedule_bg
                Layout.fillWidth: true                
                Layout.fillHeight: true
                width: layout.width
                height: layout.height / 2
                color: "#1D2025"
                radius: 10
                property bool ready: schedule_handler.finished && schedule_handler.success

                Icon {
                    id: save_schedule
                    anchors {
                        top: parent.top
                        left: parent.left
                        leftMargin: 10
                        topMargin: 10
                    }
                    text: "\u1f4b"
                    color: "#FFFFFF"
                    description: "ذخیره برنامه هفتگی"
                    clickAble: true
                    onClicked: file_dialog.saveItem(table_schedule_bg, save_schedule)
                }

                Item {
                    id: blank_space
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: 90
                    height: 40
                    Label {
                        anchors.centerIn: parent
                        text: "روز / ساعت"
                        font.family: "Sahel"
                        color: "#FFFFFF"
                    }
                }

                RowLayout {
                    id: hours
                    y: -2
                    layoutDirection: Qt.RightToLeft
                    width: parent.width - blank_space.width
                    height: blank_space.height
                    spacing: 0
                    Repeater {
                        id: hours_repeater
                        model: ["۸-۱۰", "۱۰-۱۲", "۱۳-۱۵", "۱۵-۱۷", "۱۷-۱۹"]
                        Rectangle {
                            Layout.alignment: Qt.AlignRight
                            color: "transparent"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Label {
                                anchors.centerIn: parent
                                font.family: "Sahel"
                                color: "#FFFFFF"
                                text: modelData
                            }
                            Rectangle {
                                x: parent.width
                                width: 2
                                height: parent.height
                                color: "#262A2F"
                            }
                        }
                    }
                }

                ColumnLayout {
                    id: days
                    anchors.right: parent.right
                    anchors.top: blank_space.bottom
                    anchors.topMargin: -4
                    spacing: 0
                    width: blank_space.width
                    height: table_schedule_bg.height - blank_space.height + 4
                    Repeater {
                        id: days_repeater
                        model: ["شنبه", "یکشنبه", "دوشنبه", "سه‌شنبه", "چهارشنبه"]
                        Rectangle {
                            Layout.alignment: Qt.AlignRight
                            color: "transparent"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Label {
                                anchors.centerIn: parent
                                font.family: "Sahel"
                                color: "#FFFFFF"
                                text: modelData
                            }
                            Rectangle {
                                y: 0
                                width: parent.width
                                height: 2
                                color: "#262A2F"
                            }
                        }
                    }
                }

                ColumnLayout {
                    id: courses
                    anchors.top: hours.bottom
                    anchors.topMargin: -1
                    width: parent.width - days.width
                    height: parent.height - hours.height
                    spacing: 0
                    Repeater {
                        id: day_row
                        model: 5
                        RowLayout {
                            width: parent.width
                            Layout.fillHeight: true
                            layoutDirection: Qt.RightToLeft
                            spacing: 0
                            Repeater {
                                model: table_schedule_bg.ready ? schedule_handler.dailyScheduleModel(index) : 0
                                Rectangle {
                                    Layout.alignment: Qt.AlignRight
                                    color: "transparent"
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                    ToolTip.visible: course_area.containsMouse && modelData.name != ""
                                    ToolTip.delay: 500
                                    ToolTip.text: "استاد: " + modelData.teacher + "<br>تاریخ امتحان:‌ " + modelData.exam
                                    Label {
                                        width: parent.width - 5
                                        anchors.centerIn: parent
                                        font.family: "Sahel"
                                        color: "#FFFFFF"
                                        text: modelData.name
                                        wrapMode: Label.WordWrap
                                        horizontalAlignment: Label.AlignHCenter
                                    }
                                    MouseArea {
                                        id: course_area
                                        anchors.fill: parent
                                        hoverEnabled: true
                                    }

                                    Rectangle {
                                        y: 0
                                        width: parent.width + 4
                                        height: 2
                                        color: "#262A2F"
                                    }
                                    Rectangle {
                                        width: 2
                                        height: parent.height + 4
                                        x: parent.width
                                        color: "#262A2F"
                                    }
                                }
                            }
                        }
                    }
                }

                LoadingAnimationColor {
                    anchors.fill: parent
                    radius: 10
                    visible: !table_schedule_bg.ready
                }

            }
        }
    }
    FileDialog {
        id: file_dialog
        selectExisting: false
        selectMultiple: false
        property var item_to_save
        property var item_caller
        function saveItem(item, caller) {
            item_to_save = item
            item_caller = caller
            file_dialog.open()
        }
        onAccepted: {
            item_caller.visible = false
            item_to_save.grabToImage(function(result) {
                result.saveToFile(String(file_dialog.fileUrl).replace("file://", ""))
                item_caller.visible = item_to_save.ready
                notifier.text = "تصویر با موفقیت ذخیره شد!"
                notifier.show()
             });
        }
    }
}
