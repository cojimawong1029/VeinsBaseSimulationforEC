
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef CPU_H_
#define CPU_H_

#include <omnetpp.h>

#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/messages/WaveServiceAdvertisement_m.h"
#include "veins/modules/messages/BasicSafetyMessage_m.h"

using namespace omnetpp;


/**
 * Aloha host; see NED file for more info.
 */
class TaskManagerDemo : public cSimpleModule
{
  private:

    //int applLayerout;
    //int aoolLayerin;

    cMessage *endTxEvent;
    int applLayerin;
    int applLayerout;
    int cpuin;
    int cpuout;
//    int memoryin;
//    int memoryout;


  public :




  public:
    TaskManagerDemo();
    virtual ~TaskManagerDemo();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMsg(cMessage* msg);
    virtual void TaskScheduling(cMessage* msg);
};

 //namespace

#endif


