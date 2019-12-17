/*
 * TxQueue.cc
 *
 *  Created on: 2019Äê7ÔÂ29ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/TxQueue.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"

Define_Module(TxQueue);

TxQueue::TxQueue() {
    endTxEvent = nullptr;
}

TxQueue::~TxQueue() {
    cancelAndDelete(endTxEvent);
}

void TxQueue::initialize() {
    endTxEvent = new cMessage("endTx");
    up = findGate("up$i");
    down = findGate("downIn");

}

void TxQueue::handleSelfMsg(cMessage* msg) {
    if (msg == endTxEvent) {
        TaskRequest * tsk = dynamic_cast<TaskRequest*>(txQueue.pop());
        send(tsk, "downOut");
        if (txQueue.isEmpty()) {
            cancelEvent(endTxEvent);}
        else {
            cChannel *txChannel =
            gate("downOut")->getTransmissionChannel();
            simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
            scheduleAt(txFinishTime, endTxEvent);
        }
    }

}



void TxQueue::sendDown(cMessage* msg) {
if (txQueue.isEmpty()) {
    cChannel *txChannel = gate("downOut")->getTransmissionChannel();
    simtime_t txFinishTime = txChannel->getTransmissionFinishTime();
    if (txFinishTime <= simTime()) {
        send(msg, "downOut");
    } else {
        txQueue.insert(msg);
        scheduleAt(txFinishTime, endTxEvent);
    }
} else {
    txQueue.insert(msg);
}

}

void TxQueue::handleMessage(cMessage *msg) {
if (msg->isSelfMessage()) {
    handleSelfMsg(msg);
} else if (msg->getArrivalGateId() == up) {
    sendDown(msg);
} else if (msg->getArrivalGateId() == down) {
    send(msg, "up$o");
}
}

