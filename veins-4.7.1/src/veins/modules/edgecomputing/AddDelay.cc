/*
 * AddDelay.cc
 *
 *  Created on: 2020Äê4ÔÂ11ÈÕ
 *      Author: cojims
 */


#include "veins/modules/edgecomputing/AddDelay.h"


Define_Module(AddDelay);

void AddDelay::initialize(){
    upOut=findGate("upOut");
    downOut=findGate("downOut");
    upIn=findGate("upIn");
    downIn=findGate("downIn");
    dataTransRate=par("dataTransRate");
    rateLossRate=par("rateLossRate");
}

void AddDelay::handleMessage(cMessage* msg) {
    if(msg->getArrivalGateId() == upIn){
        if(WSMwithSignal* wsm=dynamic_cast<WSMwithSignal*>(msg)){
            double dataRateAdapted=dataTransRate*pow(rateLossRate,wsm->getRelatedSpeed());
            double tem=wsm->getBitLength()/1024/dataRateAdapted;
            sendDelayed(wsm,tem,downOut);
        }else{
            sendDelayed(msg,0.001,downOut);
        }

    }else if(msg->getArrivalGateId() == downIn){
        if(WSMwithSignal* wsm=dynamic_cast<WSMwithSignal*>(msg)){
            double dataRateAdapted=dataTransRate*pow(rateLossRate,wsm->getRelatedSpeed());
            double tem=wsm->getBitLength()/1024/dataRateAdapted;
            sendDelayed(wsm,tem,upOut);
        }else{
            sendDelayed(msg,0.001,upOut);
        }
    }

}
