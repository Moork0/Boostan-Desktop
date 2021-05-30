import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import API.BriefInfoHandler 1.0
import API.CourseScheduleHandler 1.0
import "../Controls"
import "../Helpers"


PageBase {
    id: dashboard_page
    noDestruct: true;

    BriefInfoHandler {
        id: briefinfo_handler
        property var briefInfoData: briefInfo

        Component.onCompleted: {
            // disable navigation between pages while network-job is working
            right_pane.disableNavigator()
            start()
        }
        onFinished: {
            if (!success) {
                error_handler.raiseError(this, function(){briefinfo_handler.start()}, notifier)
                right_pane.enableNavigator()
                return;
            }

            universal_storage.studentUid = Number.fromLocaleString(Qt.locale("fa_IR"), briefInfo.id);
            universal_storage.semesters = briefinfo_handler.getRawSemesters();
            universal_storage.currentSemester = briefinfo_handler.currentYear
            schedule_handler.start(universal_storage.currentSemester);
        }
    }

    CourseScheduleHandler {
        id: schedule_handler
        onFinished: {
            // enable the navigation between pages
            right_pane.enableNavigator()
            if (!success) {
                error_handler.raiseError(this, function(){briefinfo_handler.start()}, notifier)
                return;
            }
            weekly_schedule.model = schedule_handler.getSchedule()
        }
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
        font.family: regular_font.name
    }

    ScreenShot {
        id: screenshot
        callback: function(){
            exclude_item.visible = item_to_save.ready
            notifier.text = "تصویر با موفقیت ذخیره شد!"
            notifier.show()
        }
    }

    ColumnLayout {
        id: layout
        width: parent.width - 40
        height: parent.height - 40
        anchors.centerIn: parent
        spacing: 15
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.width / 2
            spacing: 15

            Rectangle {
                id: averages_plot_bg
                Layout.preferredWidth: parent.width * 0.70
                Layout.fillHeight: true
                color: "#1D2025"
                radius: 10
                property bool ready: briefinfo_handler.finished

                Icon {
                    id: save_plot
                    visible: averages_plot_bg.ready
                    anchors { top: parent.top; left: parent.left; leftMargin: 10; topMargin: 10 }
                    text: "\u1f4b"
                    color: "#FFFFFF"
                    description: "ذخیره نمودار معدل"
                    clickAble: true
                    onClicked: screenshot.saveItem(averages_plot_bg, save_plot)
                }

                Plot {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: parent.height - 40
                    xAxis: averages_plot_bg.ready ? briefinfo_handler.getSemesterYears() : []
                    yAxis: averages_plot_bg.ready ? briefinfo_handler.getSemesterAvgs() : []
                    fontFamily: regular_font.name
                }

                LoadingAnimationColor {
                    id: avgplot_loading_anim
                    anchors.fill: parent
                    radius: 10
                    visible: !averages_plot_bg.ready
                }
                ErrorRectangle {
                    visible: !avgplot_loading_anim.visible && !briefinfo_handler.success
                    anchors.fill: parent
                    name: "نمودار معدل"
                    radius: 10
                }

            }

            // TODO: use Repeater instead of raw duplicated code.
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
                    height: layout.height / 2 - 40
                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredWidth: personal_info_bg.width
                        Layout.fillHeight: true
                        text: "نام و نام خانوادگی: " + right_pane.studentName
                        wrapMode: Text.WordWrap
                        font.family: regular_font.name
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.fillHeight: true
                        text: "دوره آموزشی:‌         " + briefinfo_handler.briefInfoData.studyType
                        font.family: regular_font.name
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.fillHeight: true
                        text: "رشته تحصیلی:      " + briefinfo_handler.briefInfoData.field
                        font.family: regular_font.name
                        color: "#FFFFFF"
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.fillHeight: true
                        text: "شماره دانشجویی:   " + briefinfo_handler.briefInfoData.id
                        font.family: regular_font.name
                        color: "#FFFFFF"
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.fillHeight: true
                        text: "واحد گذرانیده:        " + briefinfo_handler.briefInfoData.passedUnits
                        font.family: regular_font.name
                        color: "#FFFFFF"
                    }

                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.fillHeight: true
                        text: "معدل کل:             " + briefinfo_handler.briefInfoData.average
                        font.family: regular_font.name
                        color: "#FFFFFF"
                    }

                }
                LoadingAnimationColor {
                    id: personalinfo_loading_anim
                    anchors.fill: parent
                    radius: 10
                    visible: !briefinfo_handler.finished
                }
                ErrorRectangle {
                    visible: !personalinfo_loading_anim.visible && !briefinfo_handler.success
                    anchors.fill: parent
                    name: "خلاصه اطلاعات"
                    radius: 10
                }

            }

        }

        Item {
            id: table_schedule_bg
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height / 2
            width: layout.width
            height: layout.height / 2
            property bool ready: schedule_handler.finished

            ScheduleTable {
                id: weekly_schedule
                anchors.right: parent.right
                width: parent.width - save_icon_bg.width
                height: parent.height
                model: []
            }

            Rectangle {
                id: save_icon_bg
                anchors.right: weekly_schedule.left
                anchors.top: weekly_schedule.top
                anchors.topMargin: 5
                anchors.bottom: weekly_schedule.bottom
                anchors.bottomMargin: 5
                width: 30
                color: "#1D2025"
                radius: 5
                Icon {
                    id: save_schedule
                    anchors.centerIn: parent
                    text: "\u1f4b"
                    color: "#FFFFFF"
                    description: "ذخیره برنامه هفتگی"
                    clickAble: true
                    onClicked: screenshot.saveItem(table_schedule_bg, save_icon_bg)
                }
            }

            LoadingAnimationColor {
                id: schedule_loading_animation
                anchors.fill: parent
                radius: 10
                visible: !table_schedule_bg.ready
            }

            ErrorRectangle {
                visible: !schedule_loading_animation.visible && schedule_handler.is_empty
                anchors.fill: parent
                name: "برنامه هفتگی"
                radius: 10
            }
        }

    }
}
