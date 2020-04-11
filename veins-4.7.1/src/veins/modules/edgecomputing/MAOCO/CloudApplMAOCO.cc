/*
 * CloudApplMAOCO.cc
 *
 *  Created on: 2019��7��9��
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/MAOCO/CloudApplMAOCO.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include "veins/modules/edgecomputing/EdgeMessage/infoToCloud_m.h"


Define_Module(CloudApplMAOCO);

CloudApplMAOCO::CloudApplMAOCO() {
    endTxEvent = nullptr;
}

CloudApplMAOCO::~CloudApplMAOCO() {
    cancelAndDelete(endTxEvent);
    delete dso;
}

void CloudApplMAOCO::initialize() {
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


/********************MAOCO ADD***********************/
     testVector1.setName("Test Vector 1");

}

void CloudApplMAOCO::handleSelfMsg(cMessage* msg) {

    emit(price,dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc));
    maxp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc,1.0);
    minp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc,0.0);
    maxps.record(maxp);
    minps.record(minp);
    senps.record(dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, ujc));
    scheduleAt(simTime() + sendprice, sendp);



}
void  CloudApplMAOCO::receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details){
    std::string name=getSignalName(signalID);
//    if(!name.compare("test")){
//        this->ts=d;
//
//    }
    if(!name.compare("occucloud")){
        this->ujc=d;
    }


    }



void CloudApplMAOCO::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    ASSERT(tsk);
    int i=gate(tsk->getCloudGate())->getIndex();
    i=gate("connect$o",i)->getId();
   //tsk->setRiC(dso->calUnitRate(tsk->getGeneRate(),tsk->getTTh(),tsk->getGeneRate(),tsk->getBeta(),tsk->getGama()));
   //tsk->setExe(2);
    send(tsk,i);
}


void CloudApplMAOCO::finish(){
    recordScalar("Migration Times", MigrationTimes);
}

void CloudApplMAOCO::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGateId() == cpu) {
        handleTaskAck(msg);
    } else if(TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg)){

        if(tsk->getIsCloudExecution()){
            tsk->setCloudGate(tsk->getArrivalGateId());
            benefits+=(tsk->getQjC()*tsk->getRiC());
            beneHis.record(benefits);
            handleTaskRequest(tsk);
        }else if(tsk->getIsMigration()){
            int TaskrsuID=tsk->getTargetMigrationID();
            int SendGateIndex=RSUtoGates[TaskrsuID];
            tsk->setCloudToEdgeExe(1);
            MigrationTimes++;
            send(tsk,gate("connect$o",SendGateIndex)->getId());

            //Migration to Edge Servers
        }


    }else if(InfoToCloud* info = dynamic_cast<InfoToCloud*>(msg)){
        RSUtoGates.insert(std::pair<int,int>(info->getSenderID(),gate(info->getArrivalGateId())->getIndex()));
        cancelAndDelete(info);
        testVector1.record(RSUtoGates.size());


    }

}

void CloudApplMAOCO::handleTaskRequest(TaskRequest* tsk){
    send(tsk, "cpu$o");
}

