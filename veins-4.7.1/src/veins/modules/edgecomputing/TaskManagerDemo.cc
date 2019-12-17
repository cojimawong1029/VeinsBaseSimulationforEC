/*
 * TaskManagerDemo.cc
 *
 *  Created on: 2019Äê7ÔÂ9ÈÕ
 *      Author: xiaow
 */




#include "veins/modules/edgecomputing/TaskManagerDemo.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"


Define_Module(TaskManagerDemo);

TaskManagerDemo::TaskManagerDemo()
{
    endTxEvent = nullptr;
}

TaskManagerDemo::~TaskManagerDemo()
{
    cancelAndDelete(endTxEvent);
}

void TaskManagerDemo::initialize()
{
    applLayerin=findGate("applLayer$i");
    applLayerout=findGate("applLayer$o");
    cpuin=findGate("computer$i");
    cpuout=findGate("computer$o");


}

void TaskManagerDemo::handleSelfMsg(cMessage* msg){

}

void TaskManagerDemo::TaskScheduling(cMessage* msg){
    send(msg,"applLayer$o");
}


void TaskManagerDemo::handleMessage(cMessage *msg)
{

    if (msg->isSelfMessage()){
        handleSelfMsg(msg);
    } else if(msg->getArrivalGateId()==applLayerin) {
        TaskScheduling(msg);
    }

}

