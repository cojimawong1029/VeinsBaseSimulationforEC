#include <veins/modules/edgecomputing/EdgeServerApplLayer.h>
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"


Define_Module(EdgeServerApplLayer);

void EdgeServerApplLayer::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}

void EdgeServerApplLayer::onWSM(WaveShortMessage* wsm) {

    if (wsm->getPsid() == 10) {
        TaskRequest *tsk = dynamic_cast<TaskRequest*>(wsm->getObject(
                "taskRequest"));
        ASSERT(tsk);
      if (queue < 5 && tsk->getPurchexe()==1) {

            send(tsk->dup(),"cpu$o");
           // benefits+=(tsk->getQjE()*tsk->getRiE());
          //  beneHis.record(benefits);
          //  sendDelayed(tsk->dup(),0.1, "cloud$o");

        } else {
            sendDelayed(tsk->dup(),0.1, "cloud$o");
        }
    }
}

void EdgeServerApplLayer::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {

        cpu = findGate("cpu$i");
        cloud = findGate("cloud$i");

        findHost()->subscribe("queueLength", this);
        queue = 0;
        pingMsg = new cMessage("ping");
        pingInterval = par("pingInterval");
        scheduleAt(simTime(), pingMsg);



        WATCH(queue);
        WATCH(appldelay);
        WATCH(ujc);
        WATCH(pricecloud);
        //getSimulation()->getSystemModule()->subscribe("ujc",this);
        WATCH(uj);
        WATCH(test);
        WATCH(maxp);
        WATCH(minp);
        WATCH(numofcons);
        WATCH(gateind);
        //WATCH(numr);




//        alaphaj=par("alaphaj");
//        betaj=par("betaj");
//        gamaj=par("gamaj");
//        geneRate=par("geneRate");
        px=par("lowprice");
        py=par("changerate");
        tau=par("changeindictor");



        dso=new Nature(px,py,tau);

        getParentModule()->subscribe("uj", this);
        getParentModule()->subscribe("test", this);
        getSimulation()->getSystemModule()->subscribe("price",this);
  //      getParentModule()->subscribe("ujc", this);
        //ujs.setName("CPU rs");

 //       minps.setName("min ps");
 //       maxps.setName("max ps");
        senps.setName("sen ps");
        numofus.setName("connected cars");
        ujss.setName("ujss");
        ujss.record(0);
//        beneHis.setName("benefits");

//        beneHis.record(0);

        getParentModule()->subscribe(POST_MODEL_CHANGE, this);






    }

}

void  EdgeServerApplLayer::receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details){
        std::string name=getSignalName(signalID);
        if(!name.compare("uj")){
            this->uj=d;
        }
        if(!name.compare("test")){
            this->test=d;
        }
        if(!name.compare("price")){
            this->pricecloud=d;
        }



    }


void EdgeServerApplLayer::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    //tsk->setRiE(dso->calUnitRate(tsk->getGeneRate(),tsk->getTTh(),tsk->getGeneRate(),tsk->getBeta(),tsk->getGama()));
    WaveShortMessage* wsm = new WaveShortMessage();
    populateWSM(wsm);
    wsm->setWsmData("TaskACK");
    wsm->addObject(tsk);
    wsm->setPsid(10);
    sendDown(wsm);
    // applend= getSimulation()->getSimTime();
    // appldelay=applend-applstart;
}

void EdgeServerApplLayer::handleSelfMsg(cMessage* msg) {
    if (msg == pingMsg) {
        WaveShortMessage* wsm = new WaveShortMessage();
        populateWSM(wsm);

        //wsm->setWsmData("TaskACK");
        //wsm->addObject(tsk);
        wsm->setPsid(11);
        wsm->setRiC(pricecloud);
        wsm->setRiE(dso->getPrice(uj/1000));
        senps.record(dso->getPrice(uj/1000));
        ujss.record(uj/1000);
//        maxp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, uj/numr,1.0);
//        minp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, uj/numr,0.0);
//        maxps.record(maxp);
//        minps.record(minp);
//        senps.record(dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, uj/numr,0.0));
        sendDown(wsm);
        scheduleAt(simTime()+pingInterval, pingMsg);

    }
}

void EdgeServerApplLayer::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGateId() == upperLayerIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::UPPER_DATA, msg);
        handleUpperMsg(msg);
    } else if (msg->getArrivalGateId() == upperControlIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::UPPER_CONTROL,
                msg);
        handleUpperControl(msg);
    } else if (msg->getArrivalGateId() == lowerControlIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::LOWER_CONTROL,
                msg);
        handleLowerControl(msg);
    } else if (msg->getArrivalGateId() == lowerLayerIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::LOWER_DATA, msg);
        handleLowerMsg(msg);
    } else if (msg->getArrivalGateId() == cpu) {
        handleTaskAck(msg);
    } else if (msg->getArrivalGateId() == cloud) {
        handleTaskAck(msg);
    } else if (msg->getArrivalGateId() == -1) {
        throw cRuntimeError(
                "No self message and no gateID?? Check configuration.");
    } else {
        throw cRuntimeError(
                "Unknown gateID?? Check configuration or override handleMessage().");
    }
}

