/*
 * PM.cc
 *
 *  Created on: 2019年7月22日
 *      Author: xiaow
 */




/*
 * PM.cc
 *
 *  Created on: 2019年7月12日
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/VM/PM.h"
#include <math.h>



PM::PM(){
    this->ram=300;
    this->mips=1e6;
    this->lastencaltime=0;
}

PM::PM(double mips,double ram,double Eoff,double Eidle,double Ebusy){
    this->mips=mips;
    this->ram=ram;
    this->Eoff=Eoff;
    this->Eidle=Eidle;
    this->Ebusy=Ebusy;
    this->lastencaltime=0;
}

PM::~PM(){
    for(auto it = VMs.begin(); it != VMs.end(); it++){
        delete *it;
        it = VMs.erase(it);
        if(it == VMs.end()) break;
        }

}

double PM::getMips(){
    return this->mips;
}

double PM::getRam(){
    return this->ram;
}


void PM::setMips(double mips){
    this->mips=mips;
}

double PM::getEnergy(){
    if(getUj()>0){
        return (Ebusy-Eidle)*getUj()+Eidle;
    }else{
        return 0;
    }
}

void PM::setEoff(double Eoff){
    this->Eoff=Eoff;
}

void PM::setEidle(double Eidle){
    this->Eidle=Eidle;
}

void PM::setEbusy(double Ebusy){
    this->Ebusy=Ebusy;
}

double PM::getLoadBalance(){
    return sqrt(pow(getUj()-Tc,2.0)+pow(getRj()-Tr,2.0));
}

double PM::getPreLoadBalance(double mips,double ram){
    return sqrt(pow(getPreUj(mips)-Tc,2.0)+pow(getPreRj(ram)-Tr,2.0));
}


double PM::getUj(){
    int count=VMs.size();
    double cpuusage=0;
    for(int i=0;i<count;i++){
        cpuusage+=VMs[i]->getMips();
    }
    return cpuusage/this->mips;

}

double PM::getPreUj(double mips){
    int count=VMs.size();
    double cpuusage=0;
    for(int i=0;i<count;i++){
        cpuusage+=VMs[i]->getMips();
    }
    return (cpuusage+mips)/this->mips;

}

double PM::getRj(){
    int count=VMs.size();
    double ramusage=0;
    for(int i=0;i<count;i++){
        ramusage+=VMs[i]->getRam();
    }
    return ramusage/this->ram;
}

double PM::getPreRj(double ram){
    int count=VMs.size();
    double ramusage=0;
    for(int i=0;i<count;i++){
        ramusage+=VMs[i]->getRam();
    }
    return (ramusage+ram)/this->ram;
}

double PM::getCPUR(){
    int count=VMs.size();
    double cpuusage=0;
    for(int i=0;i<count;i++){
         cpuusage+=VMs[i]->getMips();
    }
    return this->mips-cpuusage;
}

double PM::getCPUO(){
    int count=VMs.size();
    double cpuusage=0;
    for(int i=0;i<count;i++){
         cpuusage+=VMs[i]->getMips();
    }
    return cpuusage;
}

double PM::getCPUR(double besto){
    int count=VMs.size();
    double cpuusage=0;
    for(int i=0;i<count;i++){
         cpuusage+=VMs[i]->getMips();
    }

    double tem=this->mips*besto-cpuusage;
    if(tem<0) tem=0;
    return tem;
}

double PM::getRamR(){
    int count=VMs.size();
    double ramusage=0;
    for(int i=0;i<count;i++){
         ramusage+=VMs[i]->getRam();
    }
    return this->ram-ramusage;
}

void PM::addVM(VM* vm){
    VMs.push_back(vm);
}

void PM::removeVM(VM* vm){
    for(auto it = VMs.begin(); it != VMs.end(); it++){
        if(*it == vm){
            it = VMs.erase(it);
            delete vm;
            if(it == VMs.end()) break;
        }
    }
}


VM* PM::findVMbyId(int id){
    for(int i=0;i<VMs.size();i++){
        if(VMs[i]->getId()==id){
            return VMs[i];
        }
    }
    return nullptr;

}


bool PM::isOwnerRegister(int id){
    for(int i=0;i<VMs.size();i++){
        if(VMs[i]->getVMownerId()==id){
            return true;
        }
    }
    return false;
}

void PM::setRam(double ram){
    this->ram=ram;
}

double PM::calEnCon(double time){
    double tmp=lastencaltime;
    lastencaltime=time;
    return getEnergy()*(time-tmp);

}

bool PM::isEmpty(){
    return VMs.empty();
}


