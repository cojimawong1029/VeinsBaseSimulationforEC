/*
 * VMallocationBestFit.h
 *
 *  Created on: 2019Äê7ÔÂ26ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONBESTFIT_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONBESTFIT_H_


#include "veins/modules/edgecomputing/VM/VMallocation.h"

class VMallocationBestFit : public VMallocation{
public:


    virtual bool canCreateVM(VM* vm,std::vector<PM*> PMs);
    virtual int createVMOn(VM* vm,std::vector<PM*> PMs);



};


#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMALLOCATIONBESTFIT_H_ */
