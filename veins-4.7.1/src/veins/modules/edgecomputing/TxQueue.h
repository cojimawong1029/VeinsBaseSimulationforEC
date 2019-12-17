/*
 * TxQueue.h
 *
 *  Created on: 2019Äê7ÔÂ29ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_TXQUEUE_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_TXQUEUE_H_

#include <omnetpp.h>
using namespace omnetpp;

class TxQueue : public cSimpleModule
{
  private:
    cMessage *endTxEvent;
    int up;
    int down;
    cQueue txQueue;

  public:
    TxQueue();
    virtual ~TxQueue();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void handleSelfMsg(cMessage* msg);
    virtual void sendDown(cMessage* msg);

};


#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_TXQUEUE_H_ */
