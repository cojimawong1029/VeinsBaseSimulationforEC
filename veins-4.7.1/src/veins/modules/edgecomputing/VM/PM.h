/*
 * PM.h
 *
 *  Created on: 2019Äê7ÔÂ22ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_VM_PM_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_VM_PM_H_
#include "veins/modules/edgecomputing/VM/VM.h"
#include <vector>

class PM{
public:
       PM();
       PM(double mips,double ram,double Eoff,double Eidle,double Ebusy);
       virtual ~PM();

       virtual double getMips();
       virtual double getRam();



       virtual void setMips(double mips);
       virtual void setRam(double ram);
       virtual double getEnergy();
       virtual double getLoadBalance();
       virtual double getUj();
       virtual double getRj();
       virtual double getCPUR();
       virtual double getCPUR(double besto);
       virtual double getCPUO();
       virtual double getRamR();
       virtual void setEoff(double Eoff);
       virtual void setEidle(double Eidle);
       virtual void setEbusy(double Ebusy);
       virtual void addVM(VM* vm);
       virtual void removeVM(VM* vm);
       virtual VM* findVMbyId(int id);
       virtual double calEnCon(double time);
       virtual double getPreUj(double mips);
       virtual double getPreRj(double ram);
       virtual double getPreLoadBalance(double mips,double ram);
       virtual bool isEmpty();
       virtual bool isOwnerRegister(int id);

   private:
       double ram;
       double mips;
       double Eoff;
       double Eidle;
       double Ebusy;
       double Tc=0.8; //best cpu usage;
       double Tr=1; //best ram usage;
       std::vector<VM*> VMs;
       double lastencaltime;


//       cOutVector currentUj;
//       cOutVector currentRj;


};




#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_VM_PM_H_ */
