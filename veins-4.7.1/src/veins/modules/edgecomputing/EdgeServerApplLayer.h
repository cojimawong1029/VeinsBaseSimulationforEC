#ifndef EdgeServerApplLayer_H
#define EdgeServerApplLayer_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/edgecomputing/GameTheory/Nature.h"

class EdgeServerApplLayer : public BaseWaveApplLayer {
	protected:

	int cpu;//cpu gate
	int cloud;
	simtime_t applstart;
	simtime_t applend;
	simtime_t appldelay;//application layer delay
	double queue=56;

	cMessage* pingMsg;
	double pingInterval;

    virtual void onWSM(WaveShortMessage* wsm);

    virtual void onWSA(WaveServiceAdvertisment* wsa);

    virtual void handleMessage(cMessage* msg);

    virtual void initialize(int);

    virtual void handleTaskAck(cMessage* msg);

    virtual void handleSelfMsg(cMessage* msg);


    std::map<int,simtime_t> applstarts;
    double uj{0};
    double ujc;
    double test;
    double pricecloud;


//    double alaphaj;
//    double betaj;
//    double gamaj;
//    double geneRate;
    double px;
    double py;
    double tau;

    double maxp;
    double minp;
    double numofcons{0};
    double numr{0};
   // double benefits{0};


    cOutVector maxps;
    cOutVector minps;
    cOutVector senps;
    cOutVector numofus;
    cOutVector beneHis;
    cOutVector ujss;

    double gateind{0};



    Nature* dso;
    //cOutVector ujs;

	public:

    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details){
        std::string name=getSignalName(signalID);
        if(!name.compare("queueLength")){
            queue=l;
        }
    }
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details);
    virtual void receiveSignal(cComponent *src, simsignal_t id, cObject *value,cObject *details){
        if (dynamic_cast<cPostGateAddNotification *>(value)) {
            numofcons=numofcons+1;
            numr=numofcons/6;
            gateind=gateind+10;
            numofus.record(numr);
        }
        if (dynamic_cast<cPostGateDeleteNotification *>(value)){
            numofcons=numofcons-1;
            numr=numofcons/6;
            numofus.record(numr);
    }
        if (dynamic_cast<cPostGateDisconnectNotification *>(value)){
            numofcons-=6;
            numr=numofcons/6;
            numofus.record(numr);
    }
        if (dynamic_cast<cPostGateConnectNotification *>(value)){
            if(gateind>0)gateind=gateind-12;
            else
            numofcons+=6;

            numr=numofcons/6;
            numofus.record(numr);

    }

    }

};

#endif
