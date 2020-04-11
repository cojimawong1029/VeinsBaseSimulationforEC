/*
 * CloudApplMAOCO.h
 *
 *  Created on: 2019Äê7ÔÂ9ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_CloudApplMAOCO_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_CloudApplMAOCO_H_


#include <omnetpp.h>

#include "veins/modules/edgecomputing/Informations/PsidList.h"

#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/messages/WaveServiceAdvertisement_m.h"
#include "veins/modules/messages/BasicSafetyMessage_m.h"
#include "veins/modules/edgecomputing/GameTheory/DSO.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"

#include <map>


using namespace omnetpp;


/**
 * Aloha host; see NED file for more info.
 */
class CloudApplMAOCO : public cSimpleModule,  public cListener
{
  private:

    //int applLayerout;
    //int aoolLayerin;

    cMessage *endTxEvent;
    int cpu;
    cQueue txQueue;
    double queue=56;
    simsignal_t test;
    simsignal_t price;

    double ts;

  public:



    CloudApplMAOCO();
    virtual ~CloudApplMAOCO();
    double ujc{136.8};
    virtual void finish();





  protected:
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details) {
        std::string name=getSignalName(signalID);
        if(!name.compare("queueLength")){
            queue=l;
        }
    }
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMsg(cMessage* msg);
    virtual void handleTaskAck(cMessage* msg);
    virtual void handleTaskRequest(TaskRequest* tsk);
    cMessage* sendp;
    //virtual void refreshDisplay() const override;
    //simtime_t getNextTransmissionTime();


    simtime_t sendprice;

    double alaphaj;
    double betaj;
    double gamaj;
    double geneRate;

    double maxp;
    double minp;
    double senp;
    double benefits{0};

    cOutVector maxps;
    cOutVector minps;
    cOutVector senps;
    cOutVector beneHis;


    DSO* dso;

/*****************MAOCO related*************************/
    std::map<int,int> RSUtoGates;
    int MigrationTimes{0};


/****************For Testing**************************/
    cOutVector testVector1;


};

 //namespace






#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_CloudApplMAOCO_H_ */
