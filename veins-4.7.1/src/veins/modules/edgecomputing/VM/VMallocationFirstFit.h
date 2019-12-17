/*
 * VMallocationFirstFit.h
 *
 *  Created on: 2019Äê7ÔÂ25ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONFIRSTFIT_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONFIRSTFIT_H_

#include "veins/modules/edgecomputing/VM/VMallocation.h"

class VMallocationFirstFit: public VMallocation {
public:

    virtual bool canCreateVM(VM* vm, std::vector<PM*> PMs);
    virtual int createVMOn(VM* vm, std::vector<PM*> PMs);


};

#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONFIRSTFIT_H_ */
