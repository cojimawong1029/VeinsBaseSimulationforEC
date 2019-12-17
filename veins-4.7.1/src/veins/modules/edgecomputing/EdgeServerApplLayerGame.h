#ifndef EdgeServerApplLayerGame_H
#define EdgeServerApplLayerGame_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/edgecomputing/GameTheory/DSO.h"


class EdgeServerApplLayerGame : public BaseWaveApplLayer {
	protected:

	int cpu;//cpu gate
	int cloud;
	simtime_t applstart;
	simtime_t applend;
	simtime_t appldelay;//application layer delay
	double queue;

	cMessage* pingMsg;
	double pingInterval;

    virtual void onWSM(WaveShortMessage* wsm);

    virtual void onWSA(WaveServiceAdvertisment* wsa);

    virtual void handleMessage(cMessage* msg);

    virtual void initialize(int);

    virtual void handleTaskAck(cMessage* msg);

    virtual void handleTaskAckfromCloud(cMessage* msg);

    virtual void handleSelfMsg(cMessage* msg);


    std::map<int,simtime_t> applstarts;

    DSO* dso;

	public:

    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details){
        std::string name=getSignalName(signalID);
        if(!name.compare("queueLength")){
            queue=l;
        }
    }

};

#endif
