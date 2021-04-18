import QtQuick 2.15
import QtQuick.Controls 2.15
import API.Settings 1.0
import "Helpers"
import "Controls"

ApplicationWindow {
    id: mainwindow
    visible: true
    width: 1150
    height: 700
    minimumWidth: 1000
    minimumHeight: 600
    title: "Boostan"

    FontLoader { source: "fonts/icons.ttf"; }
    FontLoader { source: "fonts/Tanha.ttf" }
    FontLoader { source: "fonts/Mj_Afsoon.ttf" }
    FontLoader { id: regular_font; source: "fonts/Vazir-Regular.ttf"; }

    // An object to store the universal necessary data's
    QtObject {
        id: universal_storage
        property var semesters: []
        property int currentSemester: 3992
        property string studentName: ""
        property string studentUid: ""
        // update prefix uid
        onStudentUidChanged: Settings.setPrefixUid(studentUid)
    }

    ViewManager {
        width: parent.width - right_pane.width
        height: parent.height
        id: stackview
        Component.onCompleted: {
            // check if there is any error occured in application initializing
            if (UniversalError) {
                error_handler.raiseUniversalError(UniversalErrorCode)
                return;
            }
            stackview.rawPush("qrc:/Pages/LoginPage.qml", "qrc:/Pages/LoginPage.qml")
        }
    }

    SideBar {
        id: right_pane
        enabled: false

        SideBarItem {
            title: "پیشخوان"
            componentPath: "qrc:/Pages/DashboardPage.qml"
        }

        SideBarItem {
            title: "دروس ارائه شده"
            componentPath: "qrc:/Pages/OfferedCoursePage.qml"
        }

        SideBarItem {
            title: "کارنامه"
            componentPath: "qrc:/Pages/ScoresPage.qml"
        }

    }

    ErrorHandler {
        id: error_handler
        viewItem: stackview
        sideBar: right_pane
    }
}
