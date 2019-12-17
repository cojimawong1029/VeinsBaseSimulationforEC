/*
 * VM.h
 *
 *  Created on: 2019Äê7ÔÂ12ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_VM_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_VM_H_

#include <iostream>

class VM{
    public:
        VM();
        VM(double mips,double ram,std::string taskmodule,int id);
        virtual ~VM();

        virtual double getMips();
        virtual double getRam();
        virtual std::string getTaskmodule();
        virtual int getId();

        int id;



        virtual void setMips(double mips);
        virtual void setRam(double ram);
        virtual void setTaskModule(std::string taskmodule);
        virtual void setId(int id);
        virtual int getVMownerId();
        virtual void setVMownerId(int id);

    private:
        double ram;
        double mips;
        std::string taskmodule;
        int VMownerId{-1};

};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_VM_H_ */
