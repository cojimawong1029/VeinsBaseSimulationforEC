/*
 * CarAppl.cc
 *
 *  Created on: 2019Äê7ÔÂ11ÈÕ
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/CarAppl.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include <random>

Define_Module(CarAppl);

CarAppl::~CarAppl() {
    cancelAndDelete(startTaskMsg);
    cancelAndDelete(stopTaskMsg);
    cancelAndDelete(checkCon);

}

void CarAppl::handleSelfMsg(cMessage* msg) {
    if (msg == startTaskMsg) {
        simtime_t TaskDuration = par("TaskDuration");
        scheduleAt(simTime() + TaskDuration, stopTaskMsg);

        TaskRequest * tsk = new TaskRequest("taskRequest");
        generateTasksCount++;

        tsk->setTaskownerid(getId());
        tsk->setTasksendid(lastTaskSendId++);
        tsk->setMi(100.0+normal(0.0, 10.0));
        tsk->setStme(10.0+normal(0.0, 1.0));
        tsk->setDyme(10.0+normal(0.0, 1.0));
        tsk->setDeadline(0.1);
        //tsk->setByteLength(1e2);

        applstart = getSimulation()->getSimTime();
        applstarts.insert(
                std::pair<int, simtime_t>(tsk->getTasksendid(), applstart));

        TaskCount--;
        //issend = true;
        if (rsudelay > 0.1) {
            send(tsk, "taskManager$o");
        } else {
            offLoad(tsk);
        }


    } else if (msg == stopTaskMsg) {
        if (TaskCount > 0) {
            simtime_t TaskInterval = par("TaskInterval");
            scheduleAt(simTime() + TaskInterval, startTaskMsg);
        }
    } else if (msg == checkCon) {
        if (getSimulation()->getSimTime().dbl() - lastCheck.dbl() >= 1) {
            rsudelay = 100;
            connectRsuId = -1;
        }
        scheduleAt(simTime() + pingInterval, checkCon);

    }

}

void CarAppl::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        TaskCount = par("TaskCount");
        taskmanagerin = findGate("taskManager$i");
        //issend = true;

        if (TaskCount > 0) {
            startTaskMsg = new cMessage("start");
            stopTaskMsg = new cMessage("stop");

            simtime_t TaskStart = par("TaskStart");
            scheduleAt(simTime() + TaskStart, startTaskMsg);
            TaskCount--;
        }
        findHost()->getDisplayString().updateWith("r=0");

        delays.setName("Appl Layer Delays");
        rsuDelays.setName("RSU Delays");

        lastTaskSendId = 0;

        applDelaysSignal = registerSignal("applDelays");
        rsuDelaySignal = registerSignal("rsuDelay");
        taskCompeleteRate=registerSignal("applCompeleteRate");

        rsudelay = 100;
        WATCH(rsudelay);
        WATCH(connectRsuId);
        checkCon = new cMessage("check ping");
        pingInterval = par("pingInterval");
        scheduleAt(simTime(), checkCon);
        lastCheck = getSimulation()->getSimTime();

    }

}

void CarAppl::handleMessage(cMessage* msg) {
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
        //delete msg;
    } else if (msg->getArrivalGateId() == taskmanagerin) {
        handleTaskAck(msg);
    } else if (msg->getArrivalGateId() == -1) {
        throw cRuntimeError(
                "No self message and no gateID?? Check configuration.");
    } else {
        throw cRuntimeError(
                "Unknown gateID?? Check configuration or override handleMessage().");
    }
}

void CarAppl::onWSA(WaveServiceAdvertisment* wsa) {

}

void CarAppl::finish() {


    recordScalar("TaskCompeleteRate", (receiveTasksCount*1.0/generateTasksCount*1.0)*100.0);
    emit(taskCompeleteRate, (receiveTasksCount*1.0/generateTasksCount*1.0)*100.0);

}


void CarAppl::onWSM(WaveShortMessage* wsm) {

    if (wsm->getPsid() == 10) {
        TaskRequest *tsk = dynamic_cast<TaskRequest*>(wsm->getObject("taskRequest"));
        ASSERT(tsk);
        if (tsk->getTaskownerid() == getId()) {
            //issend = true;
            applend = getSimulation()->getSimTime();
            appldelay = applend - applstarts[tsk->getTasksendid()];
            applstarts.erase(tsk->getTasksendid());
            delays.record(appldelay.dbl());
            emit(applDelaysSignal, appldelay.dbl());
            receiveTasksCount++;
            offLoadTasksCount++;
            if(tsk->getExe()==1){
                RiE=tsk->getRiE();
            }else if(tsk->getExe()==2){
                RiC=tsk->getRiC();
                RiE=tsk->getRiE();
            }
        }

    } else if (wsm->getPsid() == 11) {
        rsudelay = getSimulation()->getSimTime().dbl()
                - wsm->getCreationTime().dbl();
        rsuDelays.record(rsudelay);
        connectRsuId = wsm->getSenderAddress();
        emit(rsuDelaySignal, rsudelay);
        rsuID=wsm->getSenderModuleId();
    }

}

void CarAppl::offLoad(TaskRequest* tsk) {

    WaveShortMessage* wsm = new WaveShortMessage();
    populateWSM(wsm,-1, 0);
    wsm->setWsmData(mobility->getRoadId().c_str());
    wsm->addObject(tsk);
    wsm->setPsid(10);
    wsm->setBitLength(500 + normal(0.0, 100.0));
    sendDown(wsm);
}

void CarAppl::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    applend = getSimulation()->getSimTime();
    appldelay = applend - applstarts[tsk->getTasksendid()];
    applstarts.erase(tsk->getTasksendid());
    delays.record(appldelay.dbl());
    emit(applDelaysSignal, appldelay.dbl());
    receiveTasksCount++;
    delete tsk;
}

void CarAppl::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

    lastDroveAt = simTime();

}
