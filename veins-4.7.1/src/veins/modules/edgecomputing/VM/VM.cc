/*
 * VM.cc
 *
 *  Created on: 2019Äê7ÔÂ12ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/VM/VM.h"
//#include "VM.h"



VM::VM(){
    this->ram=300;
    this->mips=1e6;
    this->taskmodule="common";
}

VM::VM(double mips,double ram,std::string taskmodule,int id){
    this->mips=mips;
    this->ram=ram;
    this->taskmodule=taskmodule;
    this->id=id;
}

VM::~VM(){

}

int VM::getVMownerId(){
    return this->VMownerId;
}

void VM::setVMownerId(int id){
    this->VMownerId=id;
}

double VM::getMips(){
    return this->mips;
}

double VM::getRam(){
    return this->ram;
}

std::string VM::getTaskmodule(){
    return this->taskmodule;
}

void VM::setMips(double mips){
    this->mips=mips;
}

void VM::setRam(double ram){
    this->ram=ram;
}

void VM::setTaskModule(std::string taskmodule){
    this->taskmodule=taskmodule;
}

void VM::setId(int id){
    this->id=id;
}

int VM::getId(){
    return this->id;
}



