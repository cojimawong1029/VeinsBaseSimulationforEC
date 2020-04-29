

#include "veins/modules/edgecomputing/connectedApplications/CarApplSafe.h"

Define_Module(CarApplSafe);

void CarApplSafe::initialize(int stage) {

}

void CarApplSafe::onWSA(WaveServiceAdvertisment* wsa) {

}

void CarApplSafe::onWSM(WaveShortMessage* wsm) {

}

void CarApplSafe::handleSelfMsg(cMessage* msg) {

}

void CarApplSafe::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

}
