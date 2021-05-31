/*
    * A SideBar options Item.
    * store necessary information about an option in SideBar.
*/

import QtQuick 2.15

QtObject {
    required property string title
    required property string componentPath
    required property string iconText
    property var type: SideBarItem.Page
    enum Type
    {
        Page,
        PopUp
    }
}
