/*
 * CarApplGame.h
 *
 *  Created on: 2019Äê7ÔÂ11ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_CarApplGame_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_CarApplGame_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include "veins/modules/edgecomputing/GameTheory/DSS.h"

#include <map>

class CarApplGame : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
//        virtual void initialize();
        ~CarApplGame();
        //virtual void finish();
        virtual void finish();


    protected:




        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);
        virtual void handleMessage(cMessage* msg);
        virtual void offLoad(TaskRequest* tsk);
        virtual void handleTaskAck(cMessage* msg);
        int connectRsuId=-1;


    protected:
        int TaskCount;
        cMessage* startTaskMsg;
        cMessage* stopTaskMsg;
        cMessage* checkCon;
        DSS* dss;


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


        simsignal_t applDelaysSignal;
        simsignal_t rsuDelaySignal;
        simsignal_t taskCompeleteRate;


        cOutVector delays;
        cOutVector rsuDelays;
        cOutVector utilities;
        std::map<int,simtime_t> applstarts;

        double rsudelay;
        double alaphaj;
        double betaj;
        double gamaj;

        double geneRate;

        int rsuID;


        double RiC=-1;
        double RiE=-1;



};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_CarApplGame_H_ */
