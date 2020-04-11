/*
 * CarApplMAOCO.cc
 *
 *  Created on: 2019年7月11日
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/MAOCO/CarApplMAOCO.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include <random>
#include <math.h>

Define_Module(CarApplMAOCO);

CarApplMAOCO::~CarApplMAOCO() {
    cancelAndDelete(startTaskMsg);
    cancelAndDelete(stopTaskMsg);
    cancelAndDelete(checkCon);
    delete dss;

}


int CarApplMAOCO::choseOffloadRSU(std::map<int,Coord> RSUlocations){//MAOCO-related
    double high=RSUlocations.size();
    double randomchosen=uniform(0.0,high);
    int numofchosen=ceil(randomchosen);
//    testVector3.record(numofchosen);
    auto it=RSUlocations.begin();
    for(int i=1;i<numofchosen;i++,it++);//More judge, empty circle
    return it->first;

}


bool CarApplMAOCO::isRSUinRange(double x1,double y1,double x2,double y2,double Dmax){
    double dis=pow(pow((x1-x2),2)+pow((y1-y2),2),0.5);
    if(dis<Dmax)return true;
    else return false;
}



void CarApplMAOCO::handleSelfMsg(cMessage* msg) {
    if (msg == startTaskMsg) {

       // simtime_t TaskDuration = par("TaskDuration");
        scheduleAt(simTime() +  exponential(taskGeneInterval), stopTaskMsg);
        //scheduleAt(simTime() +  taskGeneInterval, stopTaskMsg);
        TaskRequest * tsk = new TaskRequest("taskRequest");
        //MAOCO related
        generateTasksCount++;

/*******************MAOCO ADD**********************************/
        vehicleSpeeds.record(mobility->getSpeed());
        vehicleSpeed=mobility->getSpeed();

        position=mobility->getPositionAt(simTime());
        xposition=position.x;
        yposition=position.y;
        xPositions.record(xposition);
        yPositions.record(yposition);

        for(auto it = RSUlocations.begin(); it != RSUlocations.end();){
        Coord posRSU=it->second;
        if(!isRSUinRange(posRSU.x,posRSU.y,xposition,yposition,240)){
            it=RSUlocations.erase(it);
        }else{
            it++;}
        }





/*************************************************************/

        tsk->setTaskownerid(getId());
        tsk->setTasksendid(lastTaskSendId++);
        tsk->setMi(95+normal(0.0, 10.0));
        tsk->setStme(1.0+normal(0.0, 1.0));
        tsk->setDyme(1.0+normal(0.0, 1.0));
        tsk->setDeadline(0.1);
        tsk->setGeneRate(10);
        tsk->setTTh(0.1);
        tsk->setAlapha(0.1);
        tsk->setBeta(1);
        tsk->setGama(0.001);


//        dss->setWeight(alaphaj,betaj,gamaj);
//        根据预测选择任务执行方式
        if(RiE!=-1){
            tsk->setQjE(dss->iterQ());
            tsk->setRiE(RiE);
        }
        if(RiC!=-1){
            tsk->setQjC(dss->iterQ());
            tsk->setRiC(RiC);
        }
        tsk->setPurchexe(1);
        tsk->setIsMigration(0);
        dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre(tsk->getQjE(),5000/1024.0, 27/8.0,RiE,0.02));


//        double i=dss->getUtility(tsk->getGeneRate(), 1000/tsk->getMi(),RiE,0.002,500,50);
//        double j=dss->getUtility(tsk->getGeneRate(), 1000/tsk->getMi(),RiC,0.1,500,45);
//        double k=dss->getsUtility(tsk->getGeneRate(), 1000/tsk->getMi(),0,0,0,45);

//        if(i>j&&i>k)
//            tsk->setPurchexe(1);
//        else if(j>i&&j>i){
//            tsk->setPurchexe(2);
//        }else{
//            tsk->setPurchexe(0);
//        }
        //tsk->setByteLength(1e2);

        applstart = getSimulation()->getSimTime();
        applstarts.insert(
                std::pair<int, simtime_t>(tsk->getTasksendid(), applstart));

        TaskCount--;
        //issend = true;
        if (rsudelay > 0.5||tsk->getPurchexe()==0) {
            //send(tsk, "taskManager$o");//将计算任务放到本地执行
            cancelAndDelete(tsk);
        } else {
            //cancelAndDelete(tsk);

            chosenRSUID=choseOffloadRSU(RSUlocations);
            chosenRSUIDs.record(chosenRSUID);
            offLoad(tsk,RSUlocations.begin()->first,1,0,chosenRSUID);
//            send(tsk, "taskManager$o");
        }


    } else if (msg == stopTaskMsg) {
        if (TaskCount > 0) {
            //simtime_t TaskInterval = par("TaskInterval");
            scheduleAt(simTime(), startTaskMsg);
        }
    } else if (msg == checkCon) {
        if (getSimulation()->getSimTime().dbl() - lastCheck.dbl() >= 1) {
            rsudelay = 100;
            connectRsuId = -1;
        }
        scheduleAt(simTime() + pingInterval, checkCon);

    }

}

void CarApplMAOCO::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        TaskCount = par("TaskCount");
        alaphaj=par("alaphaj");
        betaj=par("betaj");
        gamaj=par("gamaj");
        geneRate=par("geneRate");
        taskGeneInterval=par("TaskGeneInterval");
        handleRate=par("handleRate");
        tth=par("tTh");
        learnningRate=par("learnningRate");

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
        utilities.setName("Utility Values");
        puis.setName("puis");
        ByteLengths.setName("sent packet sizes");
        recePr.setName("received price");
/**********************MAOCO************************************/
        //MAOCO related
        vehicleSpeeds.setName("vehicle speeds");
        xPositions.setName("vehicle position x");
        yPositions.setName("vehicle position y");
        RSUxs.setName("RSU x positions");
        RSUys.setName("RSU y positions");
        chosenRSUIDs.setName("chosen RSU ID");
        //For Testing
        testVector1.setName("test vector 1");
        testVector2.setName("test vector 2");
        testVector3.setName("test vector 3");



 /**************************************************************/



        cc.setName("test");
        cc.record(14);

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

        dss=new Player(alaphaj,betaj,gamaj,geneRate,handleRate,tth,learnningRate);
        WATCH(RiE);
        WATCH(RiC);

        //MAOCO related
        WATCH(vehicleSpeed);
        WATCH(xposition);
        WATCH(yposition);



    }

}

void CarApplMAOCO::handleMessage(cMessage* msg) {
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

void CarApplMAOCO::onWSA(WaveServiceAdvertisment* wsa) {

}

void CarApplMAOCO::finish() {
    BaseWaveApplLayer::finish();


    recordScalar("TaskCompeleteRate", (receiveTasksCount*1.0/generateTasksCount*1.0)*100.0);
    emit(taskCompeleteRate, (receiveTasksCount*1.0/generateTasksCount*1.0)*100.0);

}


void CarApplMAOCO::onWSM(WaveShortMessage* wsm) {

    if (wsm->getPsid() == PSIDTSK) {

        TaskRequest *tsk = dynamic_cast<TaskRequest*>(wsm->getObject(
                "taskRequest"));
        ASSERT(tsk);
        if (tsk->getTaskownerid() == getId()) {
            //issend = true;
            applend = getSimulation()->getSimTime();
            appldelay = applend - applstarts[tsk->getTasksendid()];
            applstarts.erase(tsk->getTasksendid());
            delays.record(appldelay.dbl());
            emit(applDelaysSignal, appldelay.dbl());
            //dss->addPreDecision(tsk->getQjE(),dss->getUtilityPost(tsk->getQjE(),tsk->getRiE(),appldelay.dbl()));
            utilities.record(dss->getUtilityPost(tsk->getQjE(),tsk->getRiE(),appldelay.dbl()));
//            if(tsk->getPurchexe()==1)
//            utilities.record(dss->getPostUtility(10,tsk->getMi()/1000,tsk->getRiE(),appldelay.dbl(),tsk->getQjE()));
//            else
//                utilities.record(dss->getPostUtility(10,tsk->getMi()/1000,tsk->getRiC(),appldelay.dbl(),tsk->getQjC()));

            receiveTasksCount++;
            offLoadTasksCount++;

//            if(tsk->getExe()==1){
//                   RiE=tsk->getRiE();
//               }else if(tsk->getExe()==2){
//                   RiC=tsk->getRiC();
//                   RiE=tsk->getRiE();
//               }
        }

    } else if (wsm->getPsid() == BROADCASTINFO) {


/*****************MAOCAO related*****************************/

//        RSUxs.record(wsm->getTestO());
//        RSUxs.record(25.0);


        RSUposition=dynamic_cast<WSMwithSignal*>(wsm)->getSelfPosition();
        RSUxs.record(RSUposition.x);
        RSUys.record(RSUposition.y);
        connectedRSUID=wsm->getSenderAddress();

        auto pos=RSUlocations.find(connectedRSUID);
        if(pos==RSUlocations.end()? 1:0){
            RSUlocations.insert(std::pair<int,Coord>(connectedRSUID,RSUposition));
        }


        //for Test and Debug
//        testVector1.record(RSUlocations.size());
//        testVector2.record(connectedRSUID);



/***********************************************************/




        rsudelay = getSimulation()->getSimTime().dbl()
                - wsm->getCreationTime().dbl();
        //bubble("!!!!!!");

        rsuDelays.record(rsudelay);
        connectRsuId = wsm->getSenderAddress();
        emit(rsuDelaySignal, rsudelay);
        rsuID=wsm->getSenderModuleId();
        this->RiC=wsm->getRiC();
        this->RiE=wsm->getRiE();
        recePr.record(this->RiE);
    }

}

void CarApplMAOCO::offLoad(TaskRequest* tsk,int ConnectedRSUID,bool isMigration,bool isCloudExecution,int TargetM) {

    WSMwithSignal* wsm = new WSMwithSignal();
    puis.record(tsk->getQjE());
    populateWSM(wsm);
    wsm->setWsmData(mobility->getRoadId().c_str());
    wsm->addObject(tsk);
    wsm->setPsid(PSIDTSK);

/********************************MAOCO*******************************/
    wsm->setIsMigration(isMigration);
    wsm->setIsCloudExecution(isCloudExecution);
    wsm->setTargetMigrationID(TargetM);
    wsm->setConnectedRSUID(ConnectedRSUID);

/********************************************************************/




    wsm->setBitLength(5000 + normal(0.0, 100.0));
    ByteLengths.record(wsm->getByteLength());





    sendDown(wsm);
}

void CarApplMAOCO::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
//    RiC=tsk->getRiC();
//    RiE=tsk->getRiE();
    applend = getSimulation()->getSimTime();
    appldelay = applend - applstarts[tsk->getTasksendid()];
    applstarts.erase(tsk->getTasksendid());
    delays.record(appldelay.dbl());
    emit(applDelaysSignal, appldelay.dbl());
    receiveTasksCount++;
    delete tsk;
}

void CarApplMAOCO::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

    lastDroveAt = simTime();

}
