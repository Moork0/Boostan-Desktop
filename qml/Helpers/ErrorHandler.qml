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

    function raiseError(error_code, callback_function = {}, notifier = undefined)
    {
        errorCode = error_code;
        // check if error is normaal then just notify to the user
        if (criticalStatus === ErrorHandler.Normal && notifier !== undefined) {
            notifier.text = getErrorString()
            notifier.solution = getErrorSolution()
            notifier.show()
            callback_function()
            return;
        }

        /****
        TODO: showing error in pages other than LoginPage have issues with right_pane. should fix that
        ****/
        sideBar.enabled = false
        viewItem.push("../Pages/ErrorPage.qml",
        {
         "error_msg":       getErrorString(),
         "error_solution":  getErrorSolution(),
         "criticalStatus":      criticalStatus,
         "callback_function": function() {
             callback_function()
             viewItem.pop()
         }
        })
    }
}
