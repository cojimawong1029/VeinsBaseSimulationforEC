/*
 * EvaluateModule.h
 *
 *  Created on: 2019Äê7ÔÂ30ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_EVALUATEMODULE_EVALUATEMODULE_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_EVALUATEMODULE_EVALUATEMODULE_H_

#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/messages/WaveServiceAdvertisement_m.h"
#include "veins/modules/messages/BasicSafetyMessage_m.h"
#include "veins/modules/edgecomputing/VM/PM.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include <deque>
#include "veins/modules/edgecomputing/VM/VMallocation.h"


using namespace omnetpp;

class EvaluateModule : public cSimpleModule , public cListener
{
  private:
    //double delays;


  public:
    EvaluateModule();
    virtual ~EvaluateModule();
    virtual void finish();
    virtual void initialize();
    double ujc;

    //virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details);
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details);



  protected:
    cOutVector applDelays;
    cOutVector applCompeleteRate;




};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_EVALUATEMODULE_EVALUATEMODULE_H_ */
