#ifndef ABSTRACTXMLDATAHANDLER_H
#define ABSTRACTXMLDATAHANDLER_H

/*
    * Class: AbstractXmlDataHandler
    * Files: abstractxmldatahandler.h and abstractxmldatahandler.cpp
    * This is a ABSTRACT class for being drived by classes that tend to parse XML data
    * which recieved from Golestan system.

    * Note: You MUST implement 'getIsEmpty' which determines the empty-ness of a DS.
*/

#include "handler.h"
#include <QXmlStreamReader>

class AbstractXmlDataHandler : public Handler
{
    Q_OBJECT

private:
    // this property indicate that if our model is totally empty or not
    Q_PROPERTY(bool is_empty READ getIsEmpty NOTIFY isEmptyChanged)

protected:
    // REGEX pattern for extracting the XML data withing another data's
    const QString   xmldata_pattern       {QStringLiteral("<Root>[\\W\\w]+<\\/Root>")};
    bool is_empty;

public:
    AbstractXmlDataHandler();
    // set state to is_empty
    void            setIsEmpty(bool state);

    virtual bool    getIsEmpty() const = 0;

signals:
    void isEmptyChanged();
};

#endif // ABSTRACTXMLDATAHANDLER_H
