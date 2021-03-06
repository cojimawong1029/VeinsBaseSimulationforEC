/*
 * Computer.h
 *
 *  Created on: 2019��7��10��
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_COMPUTER_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_COMPUTER_H_


#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/messages/WaveServiceAdvertisement_m.h"
#include "veins/modules/messages/BasicSafetyMessage_m.h"
#include "veins/modules/edgecomputing/VM/PM.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include <deque>
#include "veins/modules/edgecomputing/VM/VMallocation.h"
#include "veins/modules/edgecomputing/VM/VMallocationBatchP.h"


using namespace omnetpp;

class Computer : public cSimpleModule
{
  private:

    double mips;
    double ram;
    double Eoff;
    double Eidle;
    double Ebusy;
    double timeIndex;
    int numofHost;

    std::vector<std::string> modules;
    std::vector<PM*> PMs;

    int applLayerin;
    int applLayerout;
    double t;
    cMessage *endTxEvent;
    int lastVmId;


  public :
    double encosum;
    Computer();
    virtual ~Computer();
    virtual void finish() override;



  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMsg(cMessage *msg);
    virtual void handleTaskRequest(TaskRequest* tsk);
    virtual int createVM(VM* vm);
    virtual void chooseFromQueue();
    void changeUsage(int i);
    double getMaxRest(double besto);

    cOutVector enconsumption;
    std::string VMAllocation;

  private:
    std::deque <TaskRequest*> queueTasks;
    std::deque <VM*> queueVMs;

    simsignal_t queueLengthSignal;
    simsignal_t bflag;
    int queueLength;

    cOutVector cpu1;
    cOutVector cpu2;
    cOutVector cpu3;
    cOutVector cpu4;
    cOutVector ram1;
    cOutVector ram2;
    cOutVector ram3;
    cOutVector ram4;






    VMallocation* vmAllocation;

    VMallocationBatchP* batchp;






};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_COMPUTER_H_ */
