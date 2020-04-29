
#include "veins/modules/EdgeComputing/connectedApplications/CarApplSafe.h"

using namespace veins;

Define_Module(CarApplSafe);

void CarApplSafe::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if(stage==0){


    }

}

void CarApplSafe::onWSA(DemoServiceAdvertisment* wsa) {
        cancelAndDelete(wsa);
}

void CarApplSafe::onWSM(BaseFrame1609_4* wsm) {


    cancelAndDelete(wsm);

}

void CarApplSafe::handleSelfMsg(cMessage* msg) {
        cancelAndDelete(msg);
}

void CarApplSafe::handlePositionUpdate(cObject* obj) {


}
