

#ifndef CarApplSafe_H
#define CarApplSafe_H

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"

namespace veins {

class CarApplSafe : public DemoBaseApplLayer {
	public:
		virtual void initialize(int stage);




	protected:
        virtual void onWSM(BaseFrame1609_4* wsm);
        virtual void onWSA(DemoServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
		virtual void handlePositionUpdate(cObject* obj);
};
}
#endif
