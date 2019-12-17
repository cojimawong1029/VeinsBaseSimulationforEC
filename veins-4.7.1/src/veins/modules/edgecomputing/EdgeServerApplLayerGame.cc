#include "veins/modules/edgecomputing/EdgeServerApplLayerGame.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"


Define_Module(EdgeServerApplLayerGame);

void EdgeServerApplLayerGame::onWSA(WaveServiceAdvertisment* wsa) {
    //if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}

void EdgeServerApplLayerGame::onWSM(WaveShortMessage* wsm) {

    if (wsm->getPsid() == 10) {
        TaskRequest *tsk = dynamic_cast<TaskRequest*>(wsm->getObject(
                "taskRequest"));
        ASSERT(tsk);
        if (queue < 5) {
            send(tsk->dup(),"cpu$o");
            //applstart = getSimulation()->getSimTime();
        } else {
            sendDelayed(tsk->dup(),0.1, "cloud$o");
            //applstart = getSimulation()->getSimTime();
        }

    }



}

void EdgeServerApplLayerGame::initialize(int stage) {
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

        dso=new DSO();
    }

}

void EdgeServerApplLayerGame::handleTaskAckfromCloud(cMessage* msg) {
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

void EdgeServerApplLayerGame::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    //tsk->setRiE(dso->calUnitRate(tsk->getGeneRate(),tsk->getTTh(),tsk->getGeneRate(),tsk->getBeta(),tsk->getGama()));
    tsk->setExe(1);
    WaveShortMessage* wsm = new WaveShortMessage();
    populateWSM(wsm);
    wsm->setWsmData("TaskACK");
    wsm->addObject(tsk);
    wsm->setPsid(10);
    sendDown(wsm);
    // applend= getSimulation()->getSimTime();
    // appldelay=applend-applstart;
}

void EdgeServerApplLayerGame::handleSelfMsg(cMessage* msg) {
    if (msg == pingMsg) {
        WaveShortMessage* wsm = new WaveShortMessage();
        populateWSM(wsm);

        //wsm->setWsmData("TaskACK");
        //wsm->addObject(tsk);
        wsm->setPsid(11);
      //  wsm->setRiE(dso->calUnitRate(10,0.1,10,0.5,50.0,))
        sendDown(wsm);
        scheduleAt(simTime()+pingInterval, pingMsg);

    }
}

void EdgeServerApplLayerGame::handleMessage(cMessage* msg) {
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
        handleTaskAckfromCloud(msg);
    } else if (msg->getArrivalGateId() == -1) {
        throw cRuntimeError(
                "No self message and no gateID?? Check configuration.");
    } else {
        throw cRuntimeError(
                "Unknown gateID?? Check configuration or override handleMessage().");
    }
}

