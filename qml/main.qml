/*
    * This is the main page for Boostan qml.
*/

import QtQuick 2.15
import QtQuick.Controls 2.15

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
    FontLoader { source: "fonts/Sahel.ttf" }

    // whole application is managed by stackview system
    StackView {
        width: parent.width - right_pane.width
        height: parent.height
        id: stackview
        Component.onCompleted: {
            // check if there is any error occured in application initializing
            if (UniversalError) {
                error_handler.raiseError(UniversalErrorCode)
                return;
            }
            stackview.push("LoginPage.qml")
//            stackview.push("DashboardPage.qml")
//            stackview.push("Test.qml")
        }
    }

    SideBar {
        id: right_pane
        enabled: false
    }

    ErrorHandler {
        id: error_handler
        viewItem: stackview
    }
}
