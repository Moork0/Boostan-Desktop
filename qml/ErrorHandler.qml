import QtQuick 2.15
import API.Errors 1.0

Error {
    id: error_handler
    // the view system which manage pages (like stackview)
    required property var viewItem

    function raiseError(error_code, callback_function = {}, notifier)
    {
        errorCode = error_code;
        if (criticalStatus === ErrorHandler.Normal) {
            notifier.text = getErrorString()
            notifier.show()
            callback_function()
            return;
        }
        viewItem.push("ErrorPage.qml",
        {
         "error_msg":       getErrorString(),
         "error_solution":  getErrorSolution(),
         "criticalStatus":      criticalStatus,
         "callback_function": function() {
             viewItem.pop()
             callback_function()
         }
        })
    }
}
