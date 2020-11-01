import QtQuick 2.15
import API.Errors 1.0

Error {
    id: error_handler
    // the view system which manage pages (like stackview)
    required property var viewItem

    function raiseError(error_code, callback_function = {}, notifier = undefined)
    {
        errorCode = error_code;
        if (criticalStatus === ErrorHandler.Normal && notifier !== undefined) {
            notifier.text = getErrorString()
            notifier.solution = getErrorSolution()
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
             callback_function()
             viewItem.pop()
         }
        })
    }
}
