/*
 * Computer2.cc
 *
 *  Created on: 2019��7��10��
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/Computer2.h"
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include <algorithm>
#include "veins/modules/edgecomputing/VM/VMallocationFirstFit2.h"
#include "veins/modules/edgecomputing/VM/VMallocationBestFit.h"
#include "veins/modules/edgecomputing/VM/VMallocationBalanceFit.h"
#include "veins/modules/edgecomputing/VM/VMallocationBatchP.h"

Define_Module(Computer2);

Computer2::Computer2() {
    endTxEvent = nullptr;
}

Computer2::~Computer2() {
    for (auto it = PMs.begin(); it != PMs.end(); it++) {
        delete *it;
        it = PMs.erase(it);
        if (it == PMs.end())
            break;
    }

    for (auto it = queueVMs.begin(); it != queueVMs.end(); it++) {
        delete *it;
        it = queueVMs.erase(it);
        if (it == queueVMs.end())
            break;
    }

    for (auto it = queueTasks.begin(); it != queueTasks.end(); it++) {
        delete *it;
        it = queueTasks.erase(it);
        if (it == queueTasks.end())
            break;
    }

    cancelAndDelete(endTxEvent);
    delete vmAllocation;
}

void Computer2::initialize() {
    mips = par("mips");
    ram = par("ram");
    numofHost = par("numofHost");
    Eoff = par("Eoff");
    Eidle = par("Eidle");
    Ebusy = par("Ebusy");
    timeIndex = par("timeIndex");

    encosum = 0;
    lastVmId = 0;

    applLayerin = findGate("applLayer$i");
    applLayerout = findGate("applLayer$o");

    modules.push_back("common");

    for (int i = 0; i < numofHost; i++) {
        PM* pm = new PM(mips, ram, Eoff, Eidle, Ebusy);
        PMs.push_back(pm);
        //*(lastencaltime+i)=0;
    }

    endTxEvent = new cMessage("hello");
    //WATCH(PMs);
    WATCH(encosum);
    enconsumption.setName("Energy Consumption");
    enconsumption.record(encosum);

    queueLengthSignal = registerSignal("queueLength");
    bflag = registerSignal("bflag");
    queueLength = 0;
    emit(queueLengthSignal, (long) queueLength);
    WATCH(queueLength);

    cpu1.setName("CPU 1");
    cpu2.setName("CPU 2");
    cpu3.setName("CPU 3");
    cpu4.setName("CPU 4");
    ram1.setName("RAM 1");
    ram2.setName("RAM 2");
    ram3.setName("RAM 3");
    ram4.setName("RAM 4");
    cpu1.record(0.0);
    cpu2.record(0.0);
    cpu3.record(0.0);
    cpu4.record(0.0);
    ram1.record(0.0);
    ram2.record(0.0);
    ram3.record(0.0);
    ram4.record(0.0);

    VMAllocation = par("VMAllocation").stringValue();

    if (VMAllocation == "FirstFit") {
        vmAllocation = new VMallocationFirstFit2();
    } else if (VMAllocation == "BestFit") {
        vmAllocation = new VMallocationBestFit();
    } else if (VMAllocation == "BalanceFit") {
        vmAllocation = new VMallocationBalanceFit();
    } else if (VMAllocation == "BatchP") {
        vmAllocation = new VMallocationBatchP();
        batchp = dynamic_cast<VMallocationBatchP*>(vmAllocation);
    }

}




void Computer2::changeUsage(int i) {
    switch (i) {
    case 0:
        cpu1.record(PMs[i]->getUj() * 100);
        ram1.record(PMs[i]->getRj() * 100);
        break;
    case 1:
        cpu2.record(PMs[i]->getUj() * 100);
        ram2.record(PMs[i]->getRj() * 100);
        break;
    case 2:
        cpu3.record(PMs[i]->getUj() * 100);
        ram3.record(PMs[i]->getRj() * 100);
        break;
    case 3:
        cpu4.record(PMs[i]->getUj() * 100);
        ram4.record(PMs[i]->getRj() * 100);
        break;
    default:
        break;
    }
}

void Computer2::handleSelfMsg(cMessage *msg) {
    TaskRequest* wsm = dynamic_cast<TaskRequest*>(msg);
    ASSERT(wsm);
    int id = wsm->getVmId();
    int ownerId;
    for (int i = 0; i < PMs.size(); i++) {
        if (PMs[i]->findVMbyId(id) != nullptr) {

            encosum += PMs[i]->calEnCon(getSimulation()->getSimTime().dbl());
            enconsumption.record(encosum);
            changeUsage(i);
            ownerId = PMs[i]->findVMbyId(id)->getVMownerId();
            PMs[i]->removeVM(PMs[i]->findVMbyId(id));
            changeUsage(i);
        }

    }
    //place the VM in the queue into the PM when a VM is removed from a PM;
    chooseFromQueue(ownerId);
    send(wsm, "applLayer$o");

}

void Computer2::chooseFromQueue(int removeId) {
    if (!queueVMs.empty()) {
        int flag = vmAllocation->chooseFlagVM(queueTasks, PMs, queueVMs,
                removeId);
        if (flag != -1) {
            auto itt = queueTasks.begin();
            auto it = queueVMs.begin();
            for (int j = 0; j < flag; j++) {
                itt++;
                it++;
            }
            TaskRequest* tsk = dynamic_cast<TaskRequest*>(*itt);
            queueTasks.erase(itt);
            tsk->setVmId(createVM(dynamic_cast<VM*>(*it)));
            queueVMs.erase(it);
            queueLength = queueVMs.size();
            emit(queueLengthSignal, queueLength);

            scheduleAt(simTime() + t, tsk);
            chooseFromQueue(removeId);
        }

    }
}

int Computer2::createVM(VM* vm) {
    int i;
    i = vmAllocation->createVMOn(vm, PMs);

    vm->setId(lastVmId);
    encosum += PMs[i]->calEnCon(getSimulation()->getSimTime().dbl());
    enconsumption.record(encosum);
    changeUsage(i);
    PMs[i]->addVM(vm);
    changeUsage(i);
    return lastVmId++;

}

void Computer2::finish() {

    recordScalar("TotalEnergy", encosum);
    recordScalar("Remained task queues", queueVMs.size());
}

bool Computer2::checkIsRegister(int id) {
    for (int i = 0; i < PMs.size(); i++) {
        if (PMs[i]->isOwnerRegister(id)) {
            return true;
        }
    }
    return false;
}

void Computer2::handleTaskRequest(TaskRequest *tsk) {

    t = timeIndex;
    if (std::find(modules.begin(), modules.end(), tsk->getTaskmodule())
            != modules.end()) { //check whether the server can hold the task?
        VM* vm = new VM(tsk->getMi()/t , tsk->getDyme(), tsk->getTaskmodule(),
                -1);
        vm->setVMownerId(tsk->getTaskownerid());
        if (checkIsRegister(tsk->getTaskownerid())) {
            queueTasks.push_back(tsk);
            queueVMs.push_back(vm);
            queueLength = queueVMs.size();
            emit(queueLengthSignal, (long) queueLength);
        } else {
            if (vmAllocation->canCreateVM(vm, PMs)) {
                tsk->setVmId(createVM(vm));
                scheduleAt(simTime() + t, tsk);
            } else {
                queueTasks.push_back(tsk);
                queueVMs.push_back(vm);
                queueLength = queueVMs.size();
                emit(queueLengthSignal, (long) queueLength);
            }

        }

    } else {
        //operation when the host can not place the VM;
    }

}

void Computer2::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGateId() == applLayerin) {
        TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
        ASSERT(tsk);
        handleTaskRequest(tsk);
    }

}
