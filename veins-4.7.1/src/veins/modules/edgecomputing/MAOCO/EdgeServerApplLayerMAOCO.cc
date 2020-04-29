#include <veins/modules/edgecomputing/MAOCO/EdgeServerApplLayerMAOCO.h>
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include "veins/modules/edgecomputing/EdgeMessage/infoToCloud_m.h"


Define_Module(EdgeServerApplLayerMAOCO);

void EdgeServerApplLayerMAOCO::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}




void EdgeServerApplLayerMAOCO::onWSM(WaveShortMessage* wsm) {

    if (wsm->getPsid() == PSIDTSK) {
        TaskRequest *tsk = dynamic_cast<TaskRequest*>(wsm->getObject(
            "taskRequest"));
        ASSERT(tsk);
        tsk->setTimetoserver(getSimulation()->getSimTime().dbl());
        if(dynamic_cast<WSMwithSignal*>(wsm)->getConnectedRSUID()==myId){
            if(!dynamic_cast<WSMwithSignal*>(wsm)->getIsCloudExecution()){
                if(!dynamic_cast<WSMwithSignal*>(wsm)->getIsMigration()){

                    tsk->setStartComputationTime(getSimulation()->getSimTime().dbl());
                    send(tsk->dup(),"cpu$o");


            }else{
                tsk->setIsMigration(1);
                tsk->setTargetMigrationID(dynamic_cast<WSMwithSignal*>(wsm)->getTargetMigrationID());

                sendDelayed(tsk->dup(),delayToCloud,"cloud$o");

            }}else{
                tsk->setIsCloudExecution(1);
                sendDelayed(tsk->dup(),delayToCloud,"cloud$o");

            }
        }
    }


}

void EdgeServerApplLayerMAOCO::finish(){

    BaseWaveApplLayer::finish();

    recordScalar("position x", xposition);
    recordScalar("position y", yposition);

}

void EdgeServerApplLayerMAOCO::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {

        cpu = findGate("cpu$i");
        cloud = findGate("cloud$i");

        findHost()->subscribe("queueLength", this);
        queue = 0;
        pingMsg = new cMessage("ping");
        sendInfotoCloud=new cMessage("sendtocloud");


        pingInterval = par("pingInterval");
        pingStart=par("pingStart");

        scheduleAt(simTime()+pingStart, pingMsg);
        scheduleAt(simTime()+0.5*pingStart, sendInfotoCloud);
        WATCH(queue);
        WATCH(appldelay);
        WATCH(ujc);
        WATCH(pricecloud);


        RSUmobility= FindModule<BaseMobility*>::findSubModule(getParentModule());
        ASSERT(RSUmobility);

        WATCH(uj);
        WATCH(test);
        WATCH(maxp);
        WATCH(minp);
        WATCH(numofcons);
        WATCH(gateind);



        px=par("lowprice");
        py=par("changerate");
        tau=par("changeindictor");
        channelUsed=par("channelUsed");



        nature=new Nature(px,py,tau);

        getParentModule()->subscribe("uj", this);
        getParentModule()->subscribe("test", this);
        getSimulation()->getSystemModule()->subscribe("price",this);


        senps.setName("sen ps");
        numofus.setName("connected cars");
        ujss.setName("ujss");
        ujss.record(0);


        getParentModule()->subscribe(POST_MODEL_CHANGE, this);

/************MAOCO*******************************************************/
        testVector1.setName("test vector 1");
        testVector2.setName("test vector 2");
        testVector3.setName("test vector 3");

        delayToCloud=par("delayToCloud");

/***********************************************************************/
    }




}

void  EdgeServerApplLayerMAOCO::receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details){
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


void EdgeServerApplLayerMAOCO::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    tsk->setTimefromserver(getSimulation()->getSimTime().dbl());
    double cost=(getSimulation()->getSimTime().dbl()-tsk->getStartComputationTime())*tsk->getComputationprice()*tsk->getComputationamout();

    testVector1.record(cost);
    tsk->setTotalCost(tsk->getTotalCost()+cost);

    WaveShortMessage* wsm = new WaveShortMessage();
    populateWSM(wsm);
    wsm->setWsmData("TaskACK");
    wsm->addObject(tsk);
    wsm->setPsid(10);
    sendDown(wsm);
    // applend= getSimulation()->getSimTime();
    // appldelay=applend-applstart;
}

void EdgeServerApplLayerMAOCO::handleSelfMsg(cMessage* msg) {
    if (msg == pingMsg) {


        RSUposition = RSUmobility->getCurrentPosition();
        xposition=RSUposition.x;
        yposition=RSUposition.y;


        WSMwithSignal* wsm = new WSMwithSignal();
        populateWSM(wsm);

        //wsm->setWsmData("TaskACK");
        //wsm->addObject(tsk);

        Coord poistionSent=RSUposition;
        wsm->setSelfPosition(poistionSent);

        //wsm->setTestO(153.5);

        wsm->setPsid(BROADCASTINFO);
        wsm->setRiC(pricecloud);
        wsm->setRiE(nature->getPrice(uj/1000));
        //wsm->setChannelNumber(channelUsed);

        senps.record(nature->getPrice(uj/1000));
        ujss.record(uj/1000);
//        maxp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, uj/numr,1.0);
//        minp=dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, uj/numr,0.0);
//        maxps.record(maxp);
//        minps.record(minp);
//        senps.record(dso->calUnitRate(geneRate, 0.1/2, 1000.0/110,betaj , gamaj, uj/numr,0.0));
        sendDown(wsm);
        scheduleAt(simTime()+pingInterval, pingMsg);

    }else if(msg == sendInfotoCloud){
        cancelAndDelete(msg);
        InfoToCloud* info=new InfoToCloud();
        info->setSenderID(myId);
        send(info,"cloud$o");


    }
}

void EdgeServerApplLayerMAOCO::handleMessage(cMessage* msg) {
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
        if(dynamic_cast<TaskRequest*>(msg)->getCloudToEdgeExe()==0){
        handleTaskAck(msg);}
        else if(dynamic_cast<TaskRequest*>(msg)->getCloudToEdgeExe()==1){
            TaskRequest* tsk=dynamic_cast<TaskRequest*>(msg);
            tsk->setStartComputationTime(getSimulation()->getSimTime().dbl());
            tsk->setTimetoserver(getSimulation()->getSimTime().dbl());
            send(tsk->dup(),"cpu$o");
        }
    } else if (msg->getArrivalGateId() == -1) {
        throw cRuntimeError(
                "No self message and no gateID?? Check configuration.");
    } else {
        throw cRuntimeError(
                "Unknown gateID?? Check configuration or override handleMessage().");
    }
}

