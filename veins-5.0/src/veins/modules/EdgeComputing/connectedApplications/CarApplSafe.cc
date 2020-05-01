

#include "veins/modules/EdgeComputing/connectedApplications/CarApplSafe.h"
#include "veins/modules/EdgeComputing/Information/CarInfo.h"
#include "veins/modules/EdgeComputing/EdgeMessage/CarInfoWSM_m.h"
#include "veins/modules/EdgeComputing/EdgeMessage/CarControl_m.h"




#include <string>

using namespace veins;

Define_Module(veins::CarApplSafe);

#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

void CarApplSafe::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {

        senseState=new cMessage("sense state");
        senseStateInterval=par("senseStateInterval");
        scheduleAt(simTime() + senseStateInterval, senseState);
        for(int i=0;i<10;i++){
            testVector[i].setName(("test vector"+std::to_string(i)).c_str());
        }
        vehicleSpeeds.setName("vehicle Speeds");
        lanePositions.setName("lane Positions");
    }
    else if (stage == 1) {

    }
}

void CarApplSafe::finish()
{
    DemoBaseApplLayer::finish();
    cancelAndDelete(senseState);

}

void CarApplSafe::onBSM(DemoSafetyMessage* bsm)
{

}

void CarApplSafe::onWSM(BaseFrame1609_4* wsm)
{
    if(CarControl* cctrl=dynamic_cast<CarControl*>(wsm)){
        double tarSpeed=-1;
        for (int ni=0;ni<8;ni++){
            if(cctrl->getVehicleId(ni)==myId){
                tarSpeed=cctrl->getSpeed(ni);
            }
        }
        if(tarSpeed!=-1){
            traciVehicle->setSpeed(tarSpeed);
        }
    }else if(CarInfoWSM* cif=dynamic_cast<CarInfoWSM*>(wsm)){
    }
}

void CarApplSafe::onWSA(DemoServiceAdvertisment* wsa)
{

}

void CarApplSafe::handleSelfMsg(cMessage* msg)
{   if(msg==senseState){
    double tem=currentLanePosition;
    lanePositions.record(currentLanePosition=traciVehicle->getLanePosition());
    if(currentLanePosition<tem)passCrossing=true;

    if(!passCrossing){

        scheduleAt(simTime() + senseStateInterval, senseState);
        vehicleSpeeds.record(currentSpeed=mobility->getSpeed());
        std::string roadId = traciVehicle->getRoadId();
        int roadIndex;
        std::string Lane1="gneE1";
        std::string Lane2="gneE3";
        std::string Lane3="gneE5";
        std::string Lane4="gneE8";
        if(strcmp(roadId.c_str(),Lane1.c_str())==0){
            roadIndex=0;
        }else if(strcmp(roadId.c_str(),Lane2.c_str())==0){
            roadIndex=2;
        }else if(strcmp(roadId.c_str(),Lane3.c_str())==0){
            roadIndex=4;
        }else if(strcmp(roadId.c_str(),Lane4.c_str())==0){
            roadIndex=6;
        }

        CarInfo carInfo=CarInfo(currentSpeed,currentLanePosition,roadId,roadIndex,myId);
        CarInfoWSM* wsm=new CarInfoWSM();
        wsm->setCarInfo(carInfo);
        populateWSM(wsm);
        sendDown(wsm);

    }else{
    }
}

}

void CarApplSafe::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);

}
