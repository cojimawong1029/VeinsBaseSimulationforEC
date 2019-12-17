/*
 * VMallocationBestFit.cc
 *
 *  Created on: 2019Äê7ÔÂ26ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/VM/VMallocationBestFit.h"
#include <math.h>

bool VMallocationBestFit::canCreateVM(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {
            return true;
        }
    }
    return false;
}

int VMallocationBestFit::createVMOn(VM* vm, std::vector<PM*> PMs) {
    int index = -1;
    double L = 2;
    double tempL = 0;
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {
            tempL = PMs[i]->getPreLoadBalance(vm->getMips(), vm->getRam());
            if (tempL < L) {
                index = i;
                L = tempL;
            }
        }
    }
    return index;
}

