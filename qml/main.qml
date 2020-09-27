import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import API.Settings 1.0

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

    StackView {
        width: parent.width
        height: parent.height
        id: stackview
//        initialItem: "LoginPage.qml"
        Component.onCompleted: {
            if (UniversalError) {
                error_handler.raiseError(UniversalErrorCode, {}, ErrorHandler.Critical)
                return;
            }
//            stackview.push("LoginPage.qml")
            stackview.width -= right_pane.width
        }
    }

    Rectangle {
        id: right_pane
        color: "#262125"
        anchors.left: stackview.right
        width: mainwindow.width * 0.25
        height: mainwindow.height
        Rectangle {
            id: right_pane_right
            anchors.right: right_pane.right
            width: right_pane.width * 0.33
            height: right_pane.height
            radius: 10
            color: "#ED960B"
            ColumnLayout {

            }
        }

    }

    ErrorHandler {
        id: error_handler
        viewItem: stackview
    }
}
