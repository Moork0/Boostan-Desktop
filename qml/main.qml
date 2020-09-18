import QtQuick 2.15
import QtQuick.Controls 2.15
import API.LoginHandler 1.0

ApplicationWindow {
    id: mainwindow
    visible: true
    width: 1150
    height: 700
    title: "Boostan"

    FontLoader { source: "fonts/icons.ttf"; name: "icons" }

    StackView {
        width: parent.width
        height: parent.height
        id: stackview
        initialItem: "LoginPage.qml"
    }
}
