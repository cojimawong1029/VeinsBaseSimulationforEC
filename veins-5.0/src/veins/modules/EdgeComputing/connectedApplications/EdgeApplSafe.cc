
#include "veins/modules/EdgeComputing/connectedApplications/EdgeApplSafe.h"
#include "veins/modules/EdgeComputing/Index/IntIndxs.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"


using namespace veins;

Define_Module(veins::EdgeApplSafe);

void EdgeApplSafe::onWSA(DemoServiceAdvertisment*  wsa) {
    cancelAndDelete(wsa);


}

void EdgeApplSafe::handleSelfMsg(cMessage* msg){
    if(msg==broadcast){
        TraCIDemo11pMessage* wsm = new TraCIDemo11pMessage();
        populateWSM(wsm);
        sendDown(wsm);
        scheduleAt(simTime() + broadcastInterval, broadcast);
    }



}

void EdgeApplSafe::initialize(int stage){
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        broadcast=new cMessage("broadcast");
        broadcastInterval=par("broadcastInterval");

    }
}

void EdgeApplSafe::onWSM(BaseFrame1609_4* wsm) {
    cancelAndDelete(wsm);
}
