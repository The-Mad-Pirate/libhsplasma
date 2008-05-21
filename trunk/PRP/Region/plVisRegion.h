#ifndef _PLVISREGION_H
#define _PLVISREGION_H

#include "NucleusLib/pnSceneObject/plObjInterface.h"

DllClass plVisRegion : public plObjInterface {
public:
    enum { kDisable, kIsNot, kReplaceNormal, kDisableNormal };

protected:
    plKey fRegion, fVisMgr;

public:
    plVisRegion();
    virtual ~plVisRegion();

    DECLARE_CREATABLE(plVisRegion)

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);
};

#endif