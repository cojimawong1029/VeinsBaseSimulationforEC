/*
 * VMallocationBatchP.cc
 *
 *  Created on: 2019Äê9ÔÂ6ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/VM/VMallocationBatchP.h"

bool VMallocationBatchP::canHoldVM(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() > vm->getMips())&& (PMs[i]->getRamR() > vm->getRam())) {
            return true;
        }
    }
    return false;
}

bool VMallocationBatchP::canCreateVM(VM* vm, std::vector<PM*> PMs){
    if (!canHoldVM(vm,PMs)){
        return false;
    }

    if(isAllPMsEmpty(PMs)){
        flag=true;
        return true;
    }else if(flag==true){
        return true;
    }else{
        return false;
    }

}


bool VMallocationBatchP::isAllPMsEmpty(std::vector<PM*> PMs){
    for(int i = 0; i < PMs.size(); i++){
        if(!PMs[i]->isEmpty()){
            return false;
        }
    }
    return true;
}

int VMallocationBatchP::createVMOn(VM* vm, std::vector<PM*> PMs) {
    for (int i = 0; i < PMs.size(); i++) {
        if ((PMs[i]->getCPUR() > vm->getMips())
                && (PMs[i]->getRamR() > vm->getRam())) {
            return i;
        }
    }
    return -1;
}


bool VMallocationBatchP::getFlag(){
    return flag;
}

void VMallocationBatchP::setFlag(bool flag){
    this->flag=flag;
}
