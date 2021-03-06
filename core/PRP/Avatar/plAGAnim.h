/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PLAGANIM_H
#define _PLAGANIM_H

#include "PRP/Object/plSynchedObject.h"
#include "plAGApplicator.h"

class PLASMA_DLL plAGAnim : public virtual plSynchedObject {
    CREATABLE(plAGAnim, kAGAnim, plSynchedObject)

public:
    enum BodyUsage {
        kBodyUnknown, kBodyUpper, kBodyFull, kBodyLower, kBodyMax
    };

protected:
    std::vector<plAGApplicator*> fApps;
    float fBlend, fStart, fEnd;
    plString fName;
    unsigned char fEoaFlag;

public:
    plAGAnim() : fBlend(0.0f), fStart(0.0f), fEnd(0.0f), fEoaFlag(0) { }
    virtual ~plAGAnim();

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);

public:
    const std::vector<plAGApplicator*>& getApplicators() const { return fApps; }
    std::vector<plAGApplicator*>& getApplicators() { return fApps; }
    void addApplicator(plAGApplicator* app) { fApps.push_back(app); }
    void delApplicator(size_t idx);
    void clearApplicators();

    float getBlend() const { return fBlend; }
    float getStart() const { return fStart; }
    float getEnd() const { return fEnd; }
    plString getName() const { return fName; }

    void setBlend(float blend) { fBlend = blend; }
    void setStart(float start) { fStart = start; }
    void setEnd(float end) { fEnd = end; }
    void setName(const plString& name) { fName = name; }
};

class PLASMA_DLL plAgeGlobalAnim : public virtual plAGAnim {
    CREATABLE(plAgeGlobalAnim, kAgeGlobalAnim, plAGAnim)

protected:
    plString fGlobalVarName;

public:
    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);

public:
    plString getVarName() const { return fGlobalVarName; }
    void setVarName(const plString& name) { fGlobalVarName = name; }
};

#endif
