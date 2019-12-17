/*
 * VMallocationFirstFit2.h
 *
 *  Created on: 2019Äê7ÔÂ25ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMallocationFirstFit2_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMallocationFirstFit2_H_

#include "veins/modules/edgecomputing/VM/VMallocation.h"

class VMallocationFirstFit2: public VMallocation {
public:

    virtual bool canCreateVM(VM* vm, std::vector<PM*> PMs);
    virtual int createVMOn(VM* vm, std::vector<PM*> PMs);
    virtual int chooseFlagVM(std::deque<TaskRequest*> queueTasks,
            std::vector<PM*> PMs, std::deque<VM*> queueVMs,int removeid);

};

#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMallocationFirstFit2_H_ */
