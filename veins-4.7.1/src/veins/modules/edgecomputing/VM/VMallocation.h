/*
 * VMallocation.h
 *
 *  Created on: 2019Äê7ÔÂ25ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATION_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATION_H_

#include "veins/modules/edgecomputing/VM/VM.h"
#include "veins/modules/edgecomputing/VM/PM.h"
#include <vector>
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include <deque>

class VMallocation {

public:
    VMallocation() {
        ;
    }
    virtual ~VMallocation() {
        ;
    }

    virtual bool canCreateVM(VM* vm, std::vector<PM*> PMs)=0;
    virtual int createVMOn(VM* vm, std::vector<PM*> PMs)=0;
    virtual int chooseFlagVM(std::deque<TaskRequest*> queueTasks,
            std::vector<PM*> PMs, std::deque<VM*> queueVMs,int removeid) {
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

};

#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATION_H_ */
