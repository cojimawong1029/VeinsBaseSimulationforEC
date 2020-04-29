

#ifndef CarApplSafe_H
#define CarApplSafe_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"



class CarApplSafe : public BaseWaveApplLayer {
	public:
		virtual void initialize(int stage);
	protected:

	protected:
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
		virtual void handlePositionUpdate(cObject* obj);
};

#endif
