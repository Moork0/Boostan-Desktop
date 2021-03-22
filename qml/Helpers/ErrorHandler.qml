/*
    * This component are responsible to run error showing mechanism
*/

import QtQuick 2.15
import API.Errors 1.0

Error {
    id: error_handler
    // the view system which manage pages (like stackview)
    required property var viewItem
    // the SideBar component
    property var sideBar

    function raiseError(caller_object, callback_function = {}, notifier = undefined)
    {
        var critical_status = caller_object.errorType
        // check if error is normaal then just notify to the user
        if (critical_status === ErrorHandler.Normal && notifier !== undefined) {
            notifier.text = caller_object.getErrorString()
            notifier.solution = caller_object.getErrorSolution()
            notifier.show()
//            callback_function()
            return;
        }

        viewItem.push("../Pages/ErrorPage.qml",
        {
         "error_msg"        :   caller_object.getErrorString(),
         "error_solution"   :   caller_object.getErrorSolution(),
         "criticalStatus"   :   critical_status,
         "sideBarItem"      :   sideBar,
         "callback_function": function() {
             callback_function()
             viewItem.pop()
         }
        })
    }
}
