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

#include "plHKSubWorld.h"

void plHKSubWorld::read(hsStream* S, plResManager* mgr) {
    plObjInterface::read(S, mgr);
    fPosition.read(S);
}

void plHKSubWorld::write(hsStream* S, plResManager* mgr) {
    plObjInterface::write(S, mgr);
    fPosition.write(S);
}

void plHKSubWorld::IPrcWrite(pfPrcHelper* prc) {
    plObjInterface::IPrcWrite(prc);

    prc->writeSimpleTag("Position");
    fPosition.prcWrite(prc);
    prc->closeTag();
}

void plHKSubWorld::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "Position") {
        if (tag->hasChildren())
            fPosition.prcParse(tag);
    } else {
        plObjInterface::IPrcParse(tag, mgr);
    }
}
