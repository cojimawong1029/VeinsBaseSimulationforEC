/*
 * AddDelay.h
 *
 *  Created on: 2020Äê4ÔÂ11ÈÕ
 *      Author: cojims
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_ADDDELAY_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_ADDDELAY_H_

#include <omnetpp.h>
using namespace omnetpp;
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/edgecomputing/MAOCO/WSMwithSignal_m.h"

class AddDelay : public cSimpleModule
{
  private:

    int upIn;
    int downIn;
    int upOut;
    int downOut;
    double dataTransRate;
    double rateLossRate;


  public:


  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;



};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_ADDDELAY_H_ */
