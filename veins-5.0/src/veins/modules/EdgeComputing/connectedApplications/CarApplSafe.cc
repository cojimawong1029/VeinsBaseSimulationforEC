

#include "veins/modules/EdgeComputing/connectedApplications/CarApplSafe.h"
#include "veins/modules/EdgeComputing/Information/CarInfo.h"
#include "veins/modules/EdgeComputing/EdgeMessage/CarInfoWSM_m.h"
#include "veins/modules/EdgeComputing/EdgeMessage/CarControl_m.h"




#include <string>

using namespace veins;

Define_Module(veins::CarApplSafe);


#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")



void CarApplSafe::initialize(int stage)
{
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {

        UDPMATLAB=par("UDPMATLAB");
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
            receiveControl=true;
            controlSpeed=tarSpeed;
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
    vehicleSpeeds.record(currentSpeed=mobility->getSpeed());
    testVector[0].record(traciVehicle->getSigma());

    if(!passCrossing&&!receiveControl){



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


        if(UDPMATLAB){


            WORD wVersionRequested;
            WSADATA wsaData;
            int err;

            wVersionRequested = MAKEWORD(1,1);

            err = WSAStartup(wVersionRequested, &wsaData);
            if(err != 0)
            {
              return;
            }

            if(LOBYTE(wsaData.wVersion) != 1 ||
              HIBYTE(wsaData.wVersion) != 1)
            {
              WSACleanup();
              return;
            }


            SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
            SOCKET sockSrvRec = socket(AF_INET,SOCK_DGRAM,0);

            sockaddr_in  addrSrv;
            sockaddr_in  addrSrvRec;
            addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
            addrSrv.sin_family = AF_INET;
            addrSrv.sin_port = htons(6969);
            addrSrvRec.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
            addrSrvRec.sin_family = AF_INET;
            addrSrvRec.sin_port = htons(6980);
            int nResult = bind(sockSrvRec, (SOCKADDR *)&addrSrvRec, sizeof(addrSrvRec));
            testVector[5].record(nResult);

            int len = sizeof(SOCKADDR);


            char recvBuf[100];
            char sendBuf[100]="Hello Server";
            char tempBuf[100];



           sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addrSrv,len);
           //closesocket(sockSrv);
           int RecF=recvfrom(sockSrvRec,recvBuf,100,0,(SOCKADDR*)&addrSrvRec,&len);
           testVector[4].record(RecF);
           std::ofstream OsWrite("ot.txt",std::ofstream::app);
           OsWrite<<recvBuf;
           OsWrite<<RecF;
           OsWrite<<std::endl;
           OsWrite.close();



           closesocket(sockSrv);
           closesocket(sockSrvRec);
              //WSACleanup();


        }



    }else{
        traciVehicle->setSpeed(controlSpeed);
    }

    scheduleAt(simTime() + senseStateInterval, senseState);
}

}

void CarApplSafe::handlePositionUpdate(cObject* obj)
{
    DemoBaseApplLayer::handlePositionUpdate(obj);

}
