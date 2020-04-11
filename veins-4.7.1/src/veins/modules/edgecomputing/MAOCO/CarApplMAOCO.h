/*
 * CarApplMAOCO.h
 *
 *  Created on: 2019Äê7ÔÂ11ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_CarApplMAOCO_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_CarApplMAOCO_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include "veins/modules/edgecomputing/GameTheory/Player.h"
#include "veins/base/utils/Coord.h"
#include "veins/modules/edgecomputing/MAOCO/WSMwithSignal_m.h"
#include "veins/modules/edgecomputing/Informations/PsidList.h"

#include <map>

class CarApplMAOCO : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
//        virtual void initialize();
        ~CarApplMAOCO();
        //virtual void finish();
        virtual void finish();


    protected:




        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);
        virtual void handleMessage(cMessage* msg);
        virtual void offLoad(TaskRequest* tsk,int ConnectedRSUID,bool isMigration=0,bool isCloudExecution=0,int TargetM=0);
        virtual void handleTaskAck(cMessage* msg);
        int connectRsuId=-1;


    protected:
        int TaskCount;
        cMessage* startTaskMsg;
        cMessage* stopTaskMsg;
        cMessage* checkCon;
        Player* dss;


        simtime_t lastDroveAt;
        bool sentMessage;
        int currentSubscribedServiceId;
        int taskmanagerin;
        int lastTaskSendId;
        int generateTasksCount{0};
        int receiveTasksCount{0};
        int offLoadTasksCount{0};
        bool issend;
        simtime_t applstart;
        simtime_t applend;
        simtime_t appldelay;
        simtime_t lastCheck;
        simtime_t pingInterval;
        simtime_t taskGeneInterval;


        simsignal_t applDelaysSignal;
        simsignal_t rsuDelaySignal;
        simsignal_t taskCompeleteRate;


        cOutVector delays;
        cOutVector rsuDelays;
        cOutVector utilities;

        cOutVector ByteLengths;
        cOutVector puis;
        cOutVector cc;


        std::map<int,simtime_t> applstarts;
// GAME related
        double rsudelay;
        double alaphaj;
        double betaj;
        double gamaj;
        double handleRate;
        cOutVector recePr;
        double geneRate;
        double tth;
        double learnningRate;
/********************MAOCO ADD****************************/
//MAOCO related
        cOutVector vehicleSpeeds;
        double vehicleSpeed{0};
        double xposition;
        double yposition;
        Coord position;
        cOutVector xPositions;
        cOutVector yPositions;
        Coord RSUposition;
        double RSUx;
        cOutVector RSUxs;
        cOutVector RSUys;
        int chosenRSUID;
        cOutVector chosenRSUIDs;

        std::map<int,Coord> RSUlocations;
        int connectedRSUID;

        virtual int choseOffloadRSU(std::map<int,Coord> RSUlocations);



        virtual bool isRSUinRange(double x1,double y1,double x2,double y2,double Dmax);



//For Testing
        cOutVector testVector1;
        cOutVector testVector2;
        cOutVector testVector3;


/********************************************************/

        int rsuID;


        double RiC=-1;
        double RiE=-1;



};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_CarApplMAOCO_H_ */
