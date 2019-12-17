/*
 * VMP.h
 *
 *  Created on: 2019Äê7ÔÂ29ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMP_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMP_H_

class VMP{
public:
    VMP(){
        ;
    }
    virtual ~VMP(){
        ;
    }

    virtual bool canCreateVM(VM* vm,std::vector<PM*> PMs)=0;
    virtual int createVMOn(VM* vm,std::vector<PM*> PMs)=0;

};


#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_VMP_H_ */

