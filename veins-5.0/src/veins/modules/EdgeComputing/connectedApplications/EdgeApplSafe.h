

#pragma once

#include "veins/veins.h"

#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include <vector>
#include <map>

using namespace omnetpp;

namespace veins {


class VEINS_API EdgeApplSafe : public DemoBaseApplLayer {
public:
    void initialize(int stage) override;
    void finish() override;

protected:
    void onBSM(DemoSafetyMessage* bsm) override;
    void onWSM(BaseFrame1609_4* wsm) override;
    void onWSA(DemoServiceAdvertisment* wsa) override;

    void handleSelfMsg(cMessage* msg) override;
    void handlePositionUpdate(cObject* obj) override;


    cMessage* broadcast;

    cOutVector testVector[10];
    cOutVector SocketErrorIndex;

    std::vector<std::pair<int,std::pair<double,int>>> temSpeedData;
    int RId[8]{0,0,0,0,0,0,0,0};
    double RSp[8]{0,0,0,0,0,0,0,0};
    double RPo[8]{0,0,0,0,0,0,0,0};





    SOCKET sockclient;
    struct sockaddr_in addr;
    bool ServerReady=false;
};

} // namespace veins
