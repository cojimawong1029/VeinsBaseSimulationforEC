
#pragma once

#include "veins/veins.h"

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"

using namespace omnetpp;

namespace veins {


class VEINS_API CarApplSafe : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;
    void finish() override;

protected:
    void onBSM(DemoSafetyMessage* bsm) override;
    void onWSM(BaseFrame1609_4* wsm) override;
    void onWSA(DemoServiceAdvertisment* wsa) override;

    void handleSelfMsg(cMessage* msg) override;
    void handlePositionUpdate(cObject* obj) override;

    cOutVector testVector[10];


/********STATES***************************************************/
    cOutVector vehicleSpeeds;
    cOutVector lanePositions;


    double currentSpeed;
    double currentLanePosition;


    simtime_t senseStateInterval;


    cMessage* senseState;

    SOCKET sockclient;
    struct sockaddr_in addr;


    bool passCrossing=false;





};

} // namespace veins
