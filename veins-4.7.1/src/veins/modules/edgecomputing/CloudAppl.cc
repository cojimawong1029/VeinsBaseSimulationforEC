/*
 * CloudAppl.cc
 *
 *  Created on: 2019Äê7ÔÂ9ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/CloudAppl.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"

Define_Module(CloudAppl);

CloudAppl::CloudAppl() {
    endTxEvent = nullptr;
}

CloudAppl::~CloudAppl() {
    cancelAndDelete(endTxEvent);
}

void CloudAppl::initialize() {
    cpu = findGate("cpu$i");
    endTxEvent = new cMessage("endTx");
    dso=new DSO();

}

void CloudAppl::handleSelfMsg(cMessage* msg) {

}

void CloudAppl::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    ASSERT(tsk);
    int i=gate(tsk->getCloudGate())->getIndex();
    i=gate("connect$o",i)->getId();
    send(tsk,i);
}

void CloudAppl::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGateId() == cpu) {
        handleTaskAck(msg);

    } else {
        TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
        ASSERT(tsk);
        tsk->setCloudGate(tsk->getArrivalGateId());
        send(tsk, "cpu$o");

    }

}

