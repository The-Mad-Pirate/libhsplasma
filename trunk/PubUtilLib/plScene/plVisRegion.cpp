#include "plVisRegion.h"

plVisRegion::plVisRegion() : fIndex(-1) {
    fProps.setName(kDisable, "kDisable");
    fProps.setName(kIsNot, "kIsNot");
    fProps.setName(kReplaceNormal, "kReplaceNormal");
    fProps.setName(kDisableNormal, "kDisableNormal");
}

plVisRegion::~plVisRegion() { }

IMPLEMENT_CREATABLE(plVisRegion, kVisRegion, plObjInterface)

void plVisRegion::read(hsStream* S, plResManager* mgr) {
    plObjInterface::read(S, mgr);
    fRegion = mgr->readKey(S);
    fVisMgr = mgr->readKey(S);
}

void plVisRegion::write(hsStream* S, plResManager* mgr) {
    plObjInterface::write(S, mgr);
    mgr->writeKey(S, fRegion);
    mgr->writeKey(S, fVisMgr);
}

void plVisRegion::IPrcWrite(pfPrcHelper* prc) {
    plObjInterface::IPrcWrite(prc);

    prc->writeSimpleTag("Region");
    fRegion->prcWrite(prc);
    prc->closeTag();
    prc->writeSimpleTag("VisMgr");
    fVisMgr->prcWrite(prc);
    prc->closeTag();
}
