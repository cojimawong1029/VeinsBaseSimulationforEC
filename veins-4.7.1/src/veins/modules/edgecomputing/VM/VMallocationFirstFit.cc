/*
 * VMallocationFirstFit.cc
 *
 *  Created on: 2019Äê7ÔÂ25ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/VM/VMallocationFirstFit.h"

bool VMallocationFirstFit::canCreateVM(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {
            return true;
        }
    }
    return false;
}

int VMallocationFirstFit::createVMOn(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {
            return i;
        }
    }
    return -1;
}


