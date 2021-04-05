#ifndef SCORESHANDLER_H
#define SCORESHANDLER_H

#include "abstractxmldatahandler.h"

class ScoresHandler final : public AbstractXmlDataHandler
{
    Q_OBJECT

private:

    // Forced implementation of AbstractXmlDataHandler pure function
    bool    getIsEmpty              () const override;

public:

    ScoresHandler();
};

#endif // SCORESHANDLER_H
