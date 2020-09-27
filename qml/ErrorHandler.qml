import QtQuick 2.15
import API.Errors 1.0

Error {
    id: error_handler
    required property var viewItem

    function raiseError(error_code, callback_function, critical_status)
    {
        errorCode = error_code;
        viewItem.push("ErrorPage.qml",
        {"error_msg":       getErrorString(),
         "error_solution":  getErrorSolution(),
         "criticalStatus":      critical_status,
         "callback_function": function() {
             viewItem.pop()
             callback_function()
         }
        })
    }
}
