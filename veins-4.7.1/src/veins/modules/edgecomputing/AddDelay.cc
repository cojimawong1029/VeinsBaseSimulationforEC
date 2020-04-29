/*
 * AddDelay.cc
 *
 *  Created on: 2020Äê4ÔÂ11ÈÕ
 *      Author: cojims
 */


#include "veins/modules/edgecomputing/AddDelay.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"


Define_Module(AddDelay);

void AddDelay::initialize(){
    upOut=findGate("upOut");
    downOut=findGate("downOut");
    upIn=findGate("upIn");
    downIn=findGate("downIn");
    dataTransRate=par("dataTransRate");
    rateLossRate=par("rateLossRate");
    Lengths.setName("Packet Lengths");
    WSMdataRates.setName("WSM data rates");
    SendWSMdelays.setName("WSM send delays");
    rateChangeInterval=par("rateChangeInterval");
    currentdataRate.setName("current data rate");
    dataRateChange=new cMessage("change data rate");
    scheduleAt(simTime() + rateChangeInterval, dataRateChange);
}

void AddDelay::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        scheduleAt(simTime() + rateChangeInterval, dataRateChange);
        double s=rand()/32767.0;
        if(s<0.25){
            dataTransRate-=1;
        }else if (s>0.75){
            dataTransRate+=1;
        }
        currentdataRate.record(dataTransRate);
    }

    if(msg->getArrivalGateId() == upIn){
        if(WSMwithSignal* wsm=dynamic_cast<WSMwithSignal*>(msg)){
            double dataRateAdapted=dataTransRate*pow(rateLossRate,wsm->getRelatedSpeed()*0.36);
            //double dataRateAdapted=dataTransRate;
            WSMdataRates.record(dataRateAdapted);
            double tem=wsm->getBitLength()/1024.0/dataRateAdapted;
            SendWSMdelays.record(tem);

            Lengths.record(wsm->getBitLength());
            wsm->setBasicDataRate(dataTransRate);
            sendDelayed(wsm,tem,downOut);
        }else{
            sendDelayed(msg,0.001,downOut);
        }

    }else if(msg->getArrivalGateId() == downIn){
        if(WSMwithSignal* wsm=dynamic_cast<WSMwithSignal*>(msg)){

            double dataRateAdapted=dataTransRate*pow(rateLossRate,wsm->getRelatedSpeed()*0.36);
            //double dataRateAdapted=dataTransRate;
            WSMdataRates.record(dataRateAdapted);
            double tem=wsm->getBitLength()/1024.0/dataRateAdapted;
            Lengths.record(wsm->getBitLength());
            SendWSMdelays.record(tem);

            if(TaskRequest *tsk = (TaskRequest*)(wsm->getObject("taskRequest")))
                tsk->setTimeTransDelay(tem);


            sendDelayed(wsm,tem,upOut);
        }else{
            sendDelayed(msg,0.001,upOut);
        }
    }

}
