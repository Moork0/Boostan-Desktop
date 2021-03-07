import QtQuick 2.15

QtObject {
    required property string title
    required property string componentPath
    property var type: SideBarItem.Page
    enum Type
    {
        Page,
        PopUp
    }
}
