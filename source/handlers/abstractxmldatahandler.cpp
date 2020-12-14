#include "header/handlers/abstractxmldatahandler.h"

AbstractXmlDataHandler::AbstractXmlDataHandler()
    : is_empty {true}
{

}

void AbstractXmlDataHandler::setIsEmpty(bool state)
{
        if (state == is_empty) return;
        is_empty = state;
        emit isEmptyChanged();
}
