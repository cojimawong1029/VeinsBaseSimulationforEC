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
}

void AddDelay::handleMessage(cMessage* msg) {
    if(msg->getArrivalGateId() == upIn){
        sendDelayed(msg,0.1,downOut);
    }else if(msg->getArrivalGateId() == downIn){
        sendDelayed(msg,0.1,upOut);
    }

}
