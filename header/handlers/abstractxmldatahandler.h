#ifndef ABSTRACTXMLDATAHANDLER_H
#define ABSTRACTXMLDATAHANDLER_H

#include "handler.h"
#include <QXmlStreamReader>

class AbstractXmlDataHandler : public Handler
{
    Q_OBJECT

private:
    // this property indicate that if our model is totally empty or not
    Q_PROPERTY(bool is_empty READ getIsEmpty NOTIFY isEmptyChanged)

protected:
    const QString   xmldata_pattern       {QStringLiteral("<Root>[\\W\\w]+<\\/Root>")};
    bool is_empty;

public:
    AbstractXmlDataHandler();
    void setIsEmpty(bool state);

    virtual bool getIsEmpty() const = 0;

signals:
    void isEmptyChanged();
};

#endif // ABSTRACTXMLDATAHANDLER_H
