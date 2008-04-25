#ifndef _PLTMCONTROLLER_H
#define _PLTMCONTROLLER_H

#include "plPosController.h"
#include "plRotController.h"
#include "plScaleController.h"

DllClass plTMController : public plController {
protected:
    plPosController* fPosController;
    plRotController* fRotController;
    plScaleController* fScaleController;

public:
    plTMController();
    virtual ~plTMController();

    DECLARE_CREATABLE(plTMController)

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);
    virtual void IPrcWrite(pfPrcHelper* prc);

    plCompoundController* convertToCompoundController();
};

#endif
