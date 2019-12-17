/*
 * VMallocationBatchP.h
 *
 *  Created on: 2019Äê9ÔÂ6ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONBATCHP_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONBATCHP_H_

#include "veins/modules/edgecomputing/VM/VMallocation.h"

class VMallocationBatchP:public VMallocation {
public:
    virtual bool canCreateVM(VM* vm, std::vector<PM*> PMs);
    virtual bool canHoldVM(VM* vm, std::vector<PM*> PMs);
    virtual int createVMOn(VM* vm, std::vector<PM*> PMs);
    virtual void setFlag(bool flag);
    virtual bool getFlag();

private:
    bool flag=false;

    virtual bool isAllPMsEmpty(std::vector<PM*> PMs);

};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONBATCHP_H_ */
