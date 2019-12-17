/*
 * CloudAppl.h
 *
 *  Created on: 2019Äê7ÔÂ9ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_CLOUDAPPL_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_CLOUDAPPL_H_


#include <omnetpp.h>

#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/messages/WaveServiceAdvertisement_m.h"
#include "veins/modules/messages/BasicSafetyMessage_m.h"
#include "veins/modules/edgecomputing/GameTheory/DSO.h"


using namespace omnetpp;


/**
 * Aloha host; see NED file for more info.
 */
class CloudAppl : public cSimpleModule
{
  private:

    //int applLayerout;
    //int aoolLayerin;

    cMessage *endTxEvent;
    int cpu;
    cQueue txQueue;

  public:
    CloudAppl();
    virtual ~CloudAppl();


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMsg(cMessage* msg);
    virtual void handleTaskAck(cMessage* msg);
    //virtual void refreshDisplay() const override;
    //simtime_t getNextTransmissionTime();

    DSO* dso;
};

 //namespace






#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_CLOUDAPPL_H_ */
