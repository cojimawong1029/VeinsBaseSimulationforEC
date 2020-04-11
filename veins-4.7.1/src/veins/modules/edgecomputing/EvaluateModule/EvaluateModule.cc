/*
 * EvaluateModule.cc
 *
 *  Created on: 2019Äê7ÔÂ30ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/EvaluateModule/EvaluateModule.h"


Define_Module(EvaluateModule);

EvaluateModule::EvaluateModule(){

}

EvaluateModule::~EvaluateModule(){

}

void EvaluateModule::finish(){

}

void  EvaluateModule::receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details){
        std::string name=getSignalName(signalID);
        if(!name.compare("applDelays")){
            //delays=d;
            applDelays.record(d);
        }else if(!name.compare("applCompeleteRate")){
            applCompeleteRate.record(d);
        }else if(!name.compare("occucloud")){
            this->ujc=d;
        }

//        else if(!name.compare("uj")){
//            this->ujc=d;
//                }
    }


void  EvaluateModule::receiveSignal(cComponent *source, simsignal_t signalID, bool d, cObject *details){
        std::string name=getSignalName(signalID);
        if(!name.compare("sigCollision")){
            colltimes.record((double)d);
        }
    }


void EvaluateModule::initialize(){
    getParentModule()->subscribe("applDelays", this);
    getParentModule()->subscribe("applCompeleteRate", this);
    getParentModule()->subscribe("occucloud", this);
    getParentModule()->subscribe("sigCollision", this);

//    getParentModule()->subscribe("queueLength", this);
    applDelays.setName("Appl Layer Delays");
    applCompeleteRate.setName("Appl Compelete Rate");
    colltimes.setName("coll times");
    WATCH(ujc);

}
