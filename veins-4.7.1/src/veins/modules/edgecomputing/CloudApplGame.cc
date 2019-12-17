/*
 * CloudApplGame.cc
 *
 *  Created on: 2019Äê7ÔÂ9ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/CloudApplGame.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"

Define_Module(CloudApplGame);

CloudApplGame::CloudApplGame() {
    endTxEvent = nullptr;
}

CloudApplGame::~CloudApplGame() {
    cancelAndDelete(endTxEvent);
    delete dso;
}

void CloudApplGame::initialize() {
    cpu = findGate("cpu$i");
    endTxEvent = new cMessage("endTx");
    dso=new DSO();
    getSimulation()->getSystemModule()->subscribe("occucloud", this);
    getSimulation()->getSystemModule()->subscribe("test", this);
    getParentModule()->subscribe("queueLength", this);
    WATCH(ujc);
    WATCH(queue);

    sendprice=par("sendprice");
    sendp=new cMessage("sendprice");
    scheduleAt(simTime() + sendprice, sendp);
    price=registerSignal("price");
    emit(price,50.0);

    alaphaj=par("alaphaj");
     betaj=par("betaj");
     gamaj=par("gamaj");
     geneRate=par("geneRate");


     WATCH(minp);
     WATCH(maxp);
//    test=registerSignal("test");
//    emit(test,55.5756);
//    WATCH(ts);


     minps.setName("min ps");
     maxps.setName("max ps");
     senps.setName("sen ps");
     beneHis.setName("benefits");

     beneHis.record(0);

}

void CloudApplGame::handleSelfMsg(cMessage* msg) {

    emit(price,dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc));
    maxp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc,1.0);
    minp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc,0.0);
    maxps.record(maxp);
    minps.record(minp);
    senps.record(dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc));
    scheduleAt(simTime() + sendprice, sendp);



}
void  CloudApplGame::receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details){
    std::string name=getSignalName(signalID);
//    if(!name.compare("test")){
//        this->ts=d;
//
//    }
    if(!name.compare("occucloud")){
        this->ujc=d;
    }


    }



void CloudApplGame::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    ASSERT(tsk);
    int i=gate(tsk->getCloudGate())->getIndex();
    i=gate("connect$o",i)->getId();
   //tsk->setRiC(dso->calUnitRate(tsk->getGeneRate(),tsk->getTTh(),tsk->getGeneRate(),tsk->getBeta(),tsk->getGama()));
   //tsk->setExe(2);
    send(tsk,i);
}

void CloudApplGame::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGateId() == cpu) {
        handleTaskAck(msg);

    } else {
        TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
        ASSERT(tsk);
        tsk->setCloudGate(tsk->getArrivalGateId());
        benefits+=(tsk->getQjC()*tsk->getRiC());
        beneHis.record(benefits);
        send(tsk, "cpu$o");

    }

}

