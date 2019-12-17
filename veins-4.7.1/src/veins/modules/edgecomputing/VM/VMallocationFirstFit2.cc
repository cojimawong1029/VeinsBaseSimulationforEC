/*
 * VMallocationFirstFit2.cc
 *
 *  Created on: 2019Äê7ÔÂ25ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/VM/VMallocationFirstFit2.h"

bool VMallocationFirstFit2::canCreateVM(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {
            return true;
        }
    }
    return false;
}

int VMallocationFirstFit2::createVMOn(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() >= vm->getMips())
                && (PMs[i]->getRamR() >= vm->getRam())) {
            return i;
        }
    }
    return -1;
}


int VMallocationFirstFit2::chooseFlagVM(std::deque<TaskRequest*> queueTasks,std::vector<PM*> PMs, std::deque<VM*> queueVMs,int removeid) {
    auto it = queueVMs.begin();
    for (int i = 0; it != queueVMs.end(); i++) {
        if (queueVMs[i]->getVMownerId()==removeid&&canCreateVM(dynamic_cast<VM*>(*it), PMs)) {
            return i;
        }
        it++;
    }

    it = queueVMs.begin();
     for (int i = 0; it != queueVMs.end(); i++) {
         if (canCreateVM(dynamic_cast<VM*>(*it), PMs)) {
             return i;
         }
         it++;
     }

     return -1;
 }
