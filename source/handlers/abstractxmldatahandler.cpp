#include "header/handlers/abstractxmldatahandler.h"

AbstractXmlDataHandler::AbstractXmlDataHandler()
    : _is_empty {true}
{

}

void AbstractXmlDataHandler::setIsEmpty(bool state)
{
        if (state == _is_empty) return;
        _is_empty = state;
        emit isEmptyChanged();
}
