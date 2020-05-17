/*
 * CarApplMAOCO.cc
 *
 *  Created on: 2019年7月11日
 *      Author: xiaow
 */

#include "veins/modules/edgecomputing/MAOCO/CarApplMAOCO.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"
#include <random>
#include <math.h>
//#include "gurobi_c++.h"
//#include <ilcplex/ilocplex.h>



Define_Module(CarApplMAOCO);

CarApplMAOCO::~CarApplMAOCO() {
    cancelAndDelete(startTaskMsg);
    cancelAndDelete(stopTaskMsg);
    cancelAndDelete(checkCon);
    //delete dss;

    for(auto it=players.begin();it!=players.end();){
        delete it->second;
        it=players.erase(it);
    }

}


int CarApplMAOCO::choseOffloadRSU(TaskRequest* tsk, int currentRSUID){//MAOCO-related
    auto pos=RSUlocations.find(currentRSUID);
    if(pos==RSUlocations.end()? 1:0||rand()/32767.0<0.1){
          double high=RSUlocations.size();
          double randomchosen=uniform(0.0,high);
          int numofchosen=ceil(randomchosen);

          auto it=RSUlocations.begin();
          for(int i=1;i<numofchosen;i++,it++);//More judge, empty circle
          int RSUID=it->first;


          dss=players[RSUID];
          double qi=dss->iterQ();
          tsk->setQjE(qi);
          tsk->setRiE(RSUprices[RSUID]);
          tsk->setPurchexe(1);




          dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre2(0.1,tsk->getComputationGain()*tsk->getComputationprice()/dss->getMu()));
          tsk->setComputationGain((0.1-5000/1024.0/200)*dss->getMu()/qi);
          tsk->setComputationprice(tsk->getRiE());
          tsk->setComputationamout(tsk->getQjE());
          tsk->setMi(tsk->getComputationGain()*tsk->getQjE()*tsk->getMi());


          return RSUID;
        }else{
            int RSUID=currentRSUID;
            dss=players[RSUID];
            double qi=dss->iterQ();
            tsk->setQjE(qi);
            tsk->setRiE(RSUprices[RSUID]);
            tsk->setPurchexe(1);

            dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre2(0.1,tsk->getComputationGain()*tsk->getComputationprice()/dss->getMu()));
            tsk->setComputationprice(tsk->getRiE());
            tsk->setComputationamout(tsk->getQjE());
            tsk->setComputationGain((0.1-5000/1024.0/200-0.05)*dss->getMu()/qi);
            tsk->setMi(tsk->getComputationGain()*tsk->getQjE()*tsk->getMi());
            return RSUID;
}


}



double CCD(double qi,double K){
    double rou=10/(qi*(100/K));
    return (1/(qi*(100/K)))+(10*((1/(qi*(100/K)))*(1/(qi*(100/K)))))/(2*(1-rou));
}


int CarApplMAOCO::choseOffloadRSUbyPassive(TaskRequest* tsk){//MAOCO-related

    auto it=RSUlocations.begin();
    int RSUID=it->first;

    dss=players[RSUID];
    double qi=dss->iterQ();
    tsk->setQjE(qi);
    tsk->setRiE(RSUprices[RSUID]);
    tsk->setPurchexe(1);
    if(RSUID==currentRSUID){
        dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre2(0.1,tsk->getComputationGain()*tsk->getComputationprice()/dss->getMu()));
        tsk->setComputationGain((0.1-5000/1024.0/200)*dss->getMu()/qi);
    }else{
        dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre2(0.1,tsk->getComputationGain()*tsk->getComputationprice()/dss->getMu()));
        tsk->setComputationGain((0.1-5000/1024.0/200-0.05)*dss->getMu()/qi);
    }

    tsk->setComputationprice(tsk->getRiE());
    tsk->setComputationamout(tsk->getQjE());

    tsk->setMi(tsk->getComputationGain()*tsk->getQjE()*tsk->getMi());



    return RSUID;

}


int CarApplMAOCO::choseOffloadRSUbyLyapunov(TaskRequest* tsk){

    double maxUti=0;
    int maxUtiRSU;
    std::vector<double> addQy;
    int id;
    double bestK=10;
    double evaluateDelay=0;
    double qq=0;
    double deW=0;
    double deSW=0;
    double deTW=0;
    double deCO=0;
    for(auto it=players.begin();it!=players.end();it++){
        dss=it->second;
        id=it->first;
        if(id==currentRSUID){
            double qi=dss->iterQ();
            double delay;
            double priceRate=RSUprices[id]/dss->getMu();
            double K=decider->findBestKbyIterawithPlayerMath2(qi,priceRate,dss);
            delay=K*tsk->getMi()/qi/10000;
            double temSW=delay;
            double nowdr=RSUdatarates[id]*pow(rateLossRate,vehicleSpeed*0.36);
            double dtt=5000/1024.0/nowdr;
            delay=delay+dtt;
            double temW=delay;
            double cost=RSUprices[id]/dss->getMu()*K;
            double P=decider->getP(delay,cost,K);
            std::vector<double> temy;
            double SUM=0;
            for (auto itt=RSUlocations.begin();itt!=RSUlocations.end();itt++){
                if(itt->first==it->first){
                    SUM=SUM+getDistance(xposition,yposition,(itt->second).x,(itt->second).y)-200;
                }else{
                    SUM=SUM+0;
                }
            }
            temy.push_back(SUM);
            double tem=decider->getLyapnovValue(P,temy);
            if(tem<=maxUti||maxUtiRSU==0){
                maxUti=tem;
                maxUtiRSU=it->first;
                addQy=temy;
                bestK=K;
                qq=qi;
                deW=temW;
                deSW=temSW;
                deTW=5000/1024.0/nowdr;
                deCO=cost;

            }
        }else{
            double qi=dss->iterQ();

            double delay;
            double priceRate=RSUprices[id]/dss->getMu()+0.1;

            double K=decider->findBestKbyIterawithPlayerMath2(qi,priceRate,dss);

            delay=K*tsk->getMi()/qi/10000;

            double temSW=delay;
            double nowdr=RSUdatarates[id]*pow(rateLossRate,vehicleSpeed*0.36);
            double dtt=5000/1024.0/nowdr;
            delay=delay+dtt+migrationDelay+2*5000/1024.0/500000*8;

            double temW=delay;

            double cost=RSUprices[id]/dss->getMu()*K+migrationCost;
            double P=decider->getP(delay,cost,K);
            std::vector<double> temy;

            double SUM=0;
            for (auto itt=RSUlocations.begin();itt!=RSUlocations.end();itt++){
                if(itt->first==it->first){
                    SUM=SUM+getDistance(xposition,yposition,(itt->second).x,(itt->second).y)-200;
                }else{
                    SUM=SUM+0;
                }
            }
            temy.push_back(SUM);
            double tem=decider->getLyapnovValue(P,temy);
            if(tem<=maxUti||maxUtiRSU==0){
                maxUti=tem;
                maxUtiRSU=it->first;
                addQy=temy;
                bestK=K;
                qq=qi;
                deSW=temSW;
                deW=temW;
                deTW=5000/1024.0/nowdr;
                deCO=cost;

            }
        }
    }



    dss=players[maxUtiRSU];
    tsk->setQjE(qq);
    tsk->setRiE(RSUprices[maxUtiRSU]);
    tsk->setPurchexe(1);
    tsk->setMi(bestK*tsk->getMi());
    tsk->setEvaluateTime(deW);
    tsk->setComputationprice(tsk->getRiE());
    tsk->setComputationamout(tsk->getQjE());
    tsk->setComputationGain(bestK);
    tsk->setEvaluateCost(deCO);
    dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre2(deW,tsk->getComputationGain()*tsk->getComputationprice()/dss->getMu()));
    addQy.push_back(deW-0.1);
    decider->updateQ(addQy);
    VirtualQueue1.record(decider->getQbyIndex(1));
    VirtualQueue2.record(decider->getQbyIndex(2));
    return maxUtiRSU;


}


std::string CarApplMAOCO::UDPServer(std::string data){
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(1,1);

    err = WSAStartup(wVersionRequested, &wsaData);
    if(err != 0)
    {
    }

    if(LOBYTE(wsaData.wVersion) != 1 ||
      HIBYTE(wsaData.wVersion) != 1)
    {
      WSACleanup();
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


    int len = sizeof(SOCKADDR);


    char recvBuf[100];
    //    char sendBuf[100]="Hello Server";
    //    char tempBuf[100];



    sendto(sockSrv,data.c_str(),strlen(data.c_str())+1,0,(SOCKADDR*)&addrSrv,len);
    //closesocket(sockSrv);
    int RecF=recvfrom(sockSrvRec,recvBuf,100,0,(SOCKADDR*)&addrSrvRec,&len);

    std::ofstream OsWrite("ot.txt",std::ofstream::app);
    OsWrite<<recvBuf;
    OsWrite<<RecF;
    OsWrite<<std::endl;
    OsWrite.close();



    closesocket(sockSrv);
    closesocket(sockSrvRec);
    return recvBuf;
}


int CarApplMAOCO::choseOffloadRSUbyLyapunovMATLAB(TaskRequest* tsk){

    double maxUti=0;
    int maxUtiRSU;
    std::vector<double> addQy;
    int id;
    double bestK=10;
    double evaluateDelay=0;
    double qq=0;
    double deW=0;
    double deSW=0;
    double deTW=0;
    double deCO=0;
    double MMK=10;

    for(auto it=players.begin();it!=players.end();it++){
        dss=it->second;
        id=it->first;
        if(id==currentRSUID){
            double qi=dss->iterQ();
            double delay;
            double priceRate=RSUprices[id]/dss->getMu();

            double nowdr=RSUdatarates[id]*pow(rateLossRate,vehicleSpeed*0.36);
            double dtt=5000/1024.0/nowdr;



            std::string sendData="";
            sendData.append(std::to_string(alphainLya));
            sendData.append(",");

            sendData.append(std::to_string(betainLya));
            sendData.append(",");

            sendData.append(std::to_string(gammainLya));
            sendData.append(",");

            sendData.append(std::to_string(dtt));
            sendData.append(",");

            sendData.append(std::to_string(dss->getMu()));
            sendData.append(",");

            sendData.append(std::to_string(qi));
            sendData.append(",");

            sendData.append(std::to_string(decider->getN()));
            sendData.append(",");

            sendData.append(std::to_string(RSUprices[id]));
            sendData.append(",");


            sendData.append(std::to_string(VinLya));
            sendData.append(",");

            sendData.append(std::to_string(decider->getQbyIndex(2)));
            std::string recev=UDPServer(sendData);
            std::stringstream ss;
            ss<<recev;
            double K;
            ss>>K;

            delay=K*tsk->getMi()/qi/10000;
            double temSW=delay;

            delay=delay+dtt;
            double temW=delay;
            double cost=RSUprices[id]/dss->getMu()*K;
            double P=decider->getP(delay,cost,K);
            std::vector<double> temy;
            double SUM=0;
            for (auto itt=RSUlocations.begin();itt!=RSUlocations.end();itt++){
                if(itt->first==it->first){
                    SUM=SUM+getDistance(xposition,yposition,(itt->second).x,(itt->second).y)-200;
                }else{
                    SUM=SUM+0;
                }
            }
            temy.push_back(SUM);
            double tem=decider->getLyapnovValue(P,temy);
            if(tem<=maxUti||maxUtiRSU==0){
                maxUti=tem;
                maxUtiRSU=it->first;
                addQy=temy;
                bestK=K;
                qq=qi;
                deW=temW;
                deSW=temSW;
                deTW=5000/1024.0/nowdr;
                deCO=cost;

            }
        }else{
            double qi=dss->iterQ();

            double delay;
            double priceRate=RSUprices[id]/dss->getMu()+0.1;

            double nowdr=RSUdatarates[id]*pow(rateLossRate,vehicleSpeed*0.36);
            double dtt=5000/1024.0/nowdr;

            std::string sendData="";
            sendData.append(std::to_string(alphainLya));
            sendData.append(",");

            sendData.append(std::to_string(betainLya));
            sendData.append(",");

            sendData.append(std::to_string(gammainLya));
            sendData.append(",");

            sendData.append(std::to_string(dtt+migrationDelay+2*5000/1024.0/500000*8));
            sendData.append(",");

            sendData.append(std::to_string(dss->getMu()));
            sendData.append(",");

            sendData.append(std::to_string(qi));
            sendData.append(",");

            sendData.append(std::to_string(decider->getN()));
            sendData.append(",");

            sendData.append(std::to_string(RSUprices[id]));
            sendData.append(",");


            sendData.append(std::to_string(VinLya));
            sendData.append(",");

            sendData.append(std::to_string(decider->getQbyIndex(2)));

            std::string recev=UDPServer(sendData);
            std::stringstream ss;
            ss<<recev;
            double K;
            ss>>K;


            delay=K*tsk->getMi()/qi/10000;

            double temSW=delay;

            delay=delay+dtt+migrationDelay+2*5000/1024.0/500000*8;

            double temW=delay;

            double cost=RSUprices[id]/dss->getMu()*K+migrationCost;
            double P=decider->getP(delay,cost,K);
            std::vector<double> temy;

            double SUM=0;
            for (auto itt=RSUlocations.begin();itt!=RSUlocations.end();itt++){
                if(itt->first==it->first){
                    SUM=SUM+getDistance(xposition,yposition,(itt->second).x,(itt->second).y)-200;
                }else{
                    SUM=SUM+0;
                }
            }
            temy.push_back(SUM);
            double tem=decider->getLyapnovValue(P,temy);
            if(tem<=maxUti||maxUtiRSU==0){
                maxUti=tem;
                maxUtiRSU=it->first;
                addQy=temy;
                bestK=K;
                qq=qi;
                deSW=temSW;
                deW=temW;
                deTW=5000/1024.0/nowdr;
                deCO=cost;


            }
        }
    }



    dss=players[maxUtiRSU];
    tsk->setQjE(qq);
    tsk->setRiE(RSUprices[maxUtiRSU]);
    tsk->setPurchexe(1);
    tsk->setMi(bestK*tsk->getMi());
    tsk->setEvaluateTime(deW);
    tsk->setComputationprice(tsk->getRiE());
    tsk->setComputationamout(tsk->getQjE());
    tsk->setComputationGain(bestK);
    tsk->setEvaluateCost(deCO);
    dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre2(deW,tsk->getComputationGain()*tsk->getComputationprice()/dss->getMu()));
    addQy.push_back(deW-0.1);
    decider->updateQ(addQy);
    VirtualQueue1.record(decider->getQbyIndex(1));
    VirtualQueue2.record(decider->getQbyIndex(2));
    testVector1.record(MMK);
    testVector2.record(bestK);

    return maxUtiRSU;


}



int CarApplMAOCO::choseOffloadRSUbyGreddy(TaskRequest* tsk){

    double maxUti=0;
    int maxUtiRSU;
    std::vector<double> addQy;
    int id;
    double bestK=10;
    double evaluateDelay=0;
    double qq=0;
    double deW=0;
    double deSW=0;
    double deTW=0;
    double deCO=0;
    for(auto it=players.begin();it!=players.end();it++){

        dss=it->second;
        id=it->first;

        if(id==currentRSUID){
            double qi=dss->iterQ();
            double delay;
            double priceRate=RSUprices[id]/dss->getMu();
            //double K=decider->findBestKbyIterawithPlayerMath3(qi,priceRate,dss);
            double nowdr=RSUdatarates[id]*pow(rateLossRate,vehicleSpeed*0.36);
            double dtt=5000/1024.0/nowdr;

            std::string sendData="";
            sendData.append(std::to_string(alphainLya));
            sendData.append(",");

            sendData.append(std::to_string(betainLya));
            sendData.append(",");

            sendData.append(std::to_string(gammainLya));
            sendData.append(",");

            sendData.append(std::to_string(dtt));
            sendData.append(",");

            sendData.append(std::to_string(dss->getMu()));
            sendData.append(",");

            sendData.append(std::to_string(qi));
            sendData.append(",");

            sendData.append(std::to_string(decider->getN()));
            sendData.append(",");

            sendData.append(std::to_string(RSUprices[id]));

            std::string recev=UDPServer(sendData);
            std::stringstream ss;
            ss<<recev;
            double K;
            ss>>K;

            delay=K*tsk->getMi()/qi/10000;
            double temSW=delay;

            delay=delay+dtt;
            double temW=delay;
            double cost=RSUprices[id]/dss->getMu()*K;
            double P=decider->getP(delay,cost,K);

            if(P<=maxUti||maxUtiRSU==0){
                maxUti=P;
                maxUtiRSU=it->first;
                bestK=K;
                qq=qi;
                deW=temW;
                deSW=temSW;
                deTW=5000/1024.0/nowdr;
                deCO=cost;

            }
        }else{
            double qi=dss->iterQ();

            double delay;
            double priceRate=RSUprices[id]/dss->getMu()+0.1;

            double nowdr=RSUdatarates[id]*pow(rateLossRate,vehicleSpeed*0.36);
            double dtt=5000/1024.0/nowdr;

            std::string sendData="";
            sendData.append(std::to_string(alphainLya));
            sendData.append(",");

            sendData.append(std::to_string(betainLya));
            sendData.append(",");

            sendData.append(std::to_string(gammainLya));
            sendData.append(",");

            sendData.append(std::to_string(dtt+migrationDelay+2*5000/1024.0/500000*8));
            sendData.append(",");

            sendData.append(std::to_string(dss->getMu()));
            sendData.append(",");

            sendData.append(std::to_string(qi));
            sendData.append(",");

            sendData.append(std::to_string(decider->getN()));
            sendData.append(",");

            sendData.append(std::to_string(RSUprices[id]));

            std::string recev=UDPServer(sendData);
            std::stringstream ss;
            ss<<recev;
            double K;
            ss>>K;



//            double K=decider->findBestKbyIterawithPlayerMath3(qi,priceRate,dss);

            delay=K*tsk->getMi()/qi/10000;

            double temSW=delay;
            delay=delay+dtt+migrationDelay+2*5000/1024.0/500000*8;


            double temW=delay;

            double cost=RSUprices[id]/dss->getMu()*K+migrationCost;
            double P=decider->getP(delay,cost,K);

            if(P<=maxUti||maxUtiRSU==0){
                maxUti=P;
                maxUtiRSU=it->first;
                bestK=K;
                qq=qi;
                deSW=temSW;
                deW=temW;
                deTW=5000/1024.0/nowdr;
                deCO=cost;

            }
        }
    }



    dss=players[maxUtiRSU];
    tsk->setQjE(qq);
    tsk->setRiE(RSUprices[maxUtiRSU]);
    tsk->setPurchexe(1);
    tsk->setMi(bestK*tsk->getMi());
    tsk->setEvaluateTime(deW);
    tsk->setComputationprice(tsk->getRiE());
    tsk->setComputationamout(tsk->getQjE());
    tsk->setComputationGain(bestK);
    tsk->setEvaluateCost(deCO);
    dss->addPreDecision(tsk->getQjE(),dss->getUtilityPre2(deW,tsk->getComputationGain()*tsk->getComputationprice()/dss->getMu()));
    return maxUtiRSU;


}


bool CarApplMAOCO::isRSUinRange(double x1,double y1,double x2,double y2,double Dmax){
    double dis=pow(pow((x1-x2),2)+pow((y1-y2),2),0.5);
    if(dis<Dmax)return true;
    else return false;
}

double CarApplMAOCO::getDistance(double x1,double y1,double x2,double y2){
    double dis=pow(pow((x1-x2),2)+pow((y1-y2),2),0.5);
    return dis;
}





void CarApplMAOCO::handleSelfMsg(cMessage* msg) {
    if (msg == startTaskMsg) {



        scheduleAt(simTime() +  exponential(taskGeneInterval), stopTaskMsg);

        TaskRequest * tsk = new TaskRequest("taskRequest");

        generateTasksCount++;

/*******************MAOCO ADD**********************************/


/*****************Information Recording***********************/
        vehicleSpeeds.record(mobility->getSpeed());
        vehicleSpeed=mobility->getSpeed();

        position=mobility->getPositionAt(simTime());
        xposition=position.x;
        yposition=position.y;
        xPositions.record(xposition);
        yPositions.record(yposition);

        for(auto it = RSUlocations.begin(); it != RSUlocations.end();){
        Coord posRSU=it->second;
        if(!isRSUinRange(posRSU.x,posRSU.y,xposition,yposition,bestDis)){
            for(auto itt=players.begin();itt!=players.end();){
                if(itt->first==it->first){
                    itt=players.erase(itt);
                }else{
                    itt++;
                }
            }
            for(auto itt=RSUprices.begin();itt!=RSUprices.end();){
                if(itt->first==it->first){
                    itt=RSUprices.erase(itt);
                }else{
                    itt++;
                }
            }

            for(auto itt=RSUdatarates.begin();itt!=RSUdatarates.end();){
                if(itt->first==it->first){
                    itt=RSUdatarates.erase(itt);
                }else{
                    itt++;
                }
            }

            int nn=1;
            for(auto itt=RSUlocations.begin();itt->first==it->first;itt++,nn++);


            it=RSUlocations.erase(it);

        }else{
            it++;}
        }


/*************************************************************/

        tsk->setTaskownerid(getId());
        tsk->setTasksendid(lastTaskSendId++);
        tsk->setMi(95+normal(0.0, 10.0));
        tsk->setStme(1.0+normal(0.0, 1.0));
        tsk->setDyme(1.0+normal(0.0, 1.0));
        tsk->setDeadline(0.1);
        tsk->setGeneRate(10);
        tsk->setTTh(0.1);
        tsk->setAlapha(0.1);
        tsk->setBeta(1);
        tsk->setGama(0.001);
        tsk->setGenerateTime(getSimulation()->getSimTime().dbl());




/*********************MAOCO related***************************/
        switch(chosenRSUway){
        case SIMPLEWAY:
            chosenRSUID=choseOffloadRSU(tsk,currentRSUID);
            break;
        case LYAPUNOVWAY:
            chosenRSUID=choseOffloadRSUbyLyapunov(tsk);
            break;
        case PASSIVEWAY:
            chosenRSUID=choseOffloadRSUbyPassive(tsk);
            break;
        case GREDDY:
            chosenRSUID=choseOffloadRSUbyGreddy(tsk);
            break;
        case LYAPUNOVMATLAB:
            chosenRSUID=choseOffloadRSUbyLyapunovMATLAB(tsk);
            break;
        default:
            chosenRSUID=choseOffloadRSU(tsk,currentRSUID);
            break;

        }


/********************choose Target Server********************/




        TaskCount--;
        //issend = true;
//        testVector4.record(rsudelay);
        if (rsudelay > 1) {
            //send(tsk, "taskManager$o");//将计算任务放到本地执行
            //cancelAndDelete(tsk);
        } else {
            //cancelAndDelete(tsk);



            chosenRSUIDs.record(chosenRSUID);
            if(chosenRSUID==currentRSUID){
            offLoad(tsk,currentRSUID,0,0,chosenRSUID);}
            else{
                offLoad(tsk,currentRSUID,1,0,chosenRSUID);
            }
            currentRSUID=chosenRSUID;
//            send(tsk, "taskManager$o");
//            testVector3.record(currentRSUID);
        }


    } else if (msg == stopTaskMsg) {
        if (TaskCount > 0) {
            //simtime_t TaskInterval = par("TaskInterval");
            scheduleAt(simTime(), startTaskMsg);
        }
    } else if (msg == checkCon) {
        if (getSimulation()->getSimTime().dbl() - lastCheck.dbl() >= 1) {
            rsudelay = 100;
            connectRsuId = -1;
        }
        scheduleAt(simTime() + pingInterval, checkCon);

    }

}

void CarApplMAOCO::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        TaskCount = par("TaskCount");
        alaphaj=par("alaphaj");
        betaj=par("betaj");
        gamaj=par("gamaj");
        geneRate=par("geneRate");
        taskGeneInterval=par("TaskGeneInterval");
        handleRate=par("handleRate");
        tth=par("tTh");
        learnningRate=par("learnningRate");
        enableUDP=par("enableUDP");

        taskmanagerin = findGate("taskManager$i");
        //issend = true;

        if (TaskCount > 0) {
            startTaskMsg = new cMessage("start");
            stopTaskMsg = new cMessage("stop");

            simtime_t TaskStart = par("TaskStart");
            scheduleAt(simTime() + TaskStart, startTaskMsg);
            TaskCount--;
        }


        findHost()->getDisplayString().updateWith("r=0");

        delays.setName("Appl Layer Delays");
        rsuDelays.setName("RSU Delays");
        utilities.setName("Utility Values");
        puis.setName("puis");
        ByteLengths.setName("sent packet sizes");
        recePr.setName("received price");
/**********************MAOCO************************************/
        //MAOCO related
        decider=new MAOCOdecider();
        VinLya=par("VinLya");
        ReV=VinLya;
        decider->setV(VinLya);
        alphainLya=par("alphainLya");
        betainLya=par("betainLya");
        gammainLya=par("gammainLya");
        bestDis=par("bestDis");
        chosenRSUway=par("chosenRSUway");
        rateLossRate=par("rateLossRate");
        migrationCost=par("migrationCost");
        migrationDelay=par("migrationDelay");
        decider->setWeight(alphainLya,betainLya,gammainLya);
        decider->addQ();
        decider->addQ();

        vehicleSpeeds.setName("vehicle speeds");
        xPositions.setName("vehicle position x");
        yPositions.setName("vehicle position y");
        RSUxs.setName("RSU x positions");
        RSUys.setName("RSU y positions");
        chosenRSUIDs.setName("chosen RSU ID");
        //For Testing
        testVector1.setName("test vector 1");
        testVector2.setName("test vector 2");
        testVector3.setName("test vector 3");
        testVector4.setName("test vector 4");
        testVector5.setName("test vector 5");
        testVector6.setName("test vector 6");
        testVector7.setName("test vector 7");
        VirtualQueue1.setName("virtual queue 1");
        VirtualQueue2.setName("virtual queue 2");
        TaskCosts.setName("task costs");
        chosenK.setName("chosen K");
        TSKevaluateDelays.setName("TSK evaluated Delays");
        TSKdelayinserver.setName("TSK delay in server");





 /**************************************************************/





        lastTaskSendId = 0;

        applDelaysSignal = registerSignal("applDelays");
        rsuDelaySignal = registerSignal("rsuDelay");
        taskCompeleteRate=registerSignal("applCompeleteRate");

        rsudelay = 100;
        WATCH(rsudelay);
        WATCH(connectRsuId);
        checkCon = new cMessage("check ping");
        pingInterval = par("pingInterval");
        scheduleAt(simTime(), checkCon);
        lastCheck = getSimulation()->getSimTime();
        WATCH(RiE);
        WATCH(RiC);

        //MAOCO related
        WATCH(vehicleSpeed);
        WATCH(xposition);
        WATCH(yposition);



    }

}

void CarApplMAOCO::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    } else if (msg->getArrivalGateId() == upperLayerIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::UPPER_DATA, msg);
        handleUpperMsg(msg);
    } else if (msg->getArrivalGateId() == upperControlIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::UPPER_CONTROL,
                msg);
        handleUpperControl(msg);
    } else if (msg->getArrivalGateId() == lowerControlIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::LOWER_CONTROL,
                msg);
        handleLowerControl(msg);
    } else if (msg->getArrivalGateId() == lowerLayerIn) {
        recordPacket(PassedMessage::INCOMING, PassedMessage::LOWER_DATA, msg);
        handleLowerMsg(msg);
        //delete msg;
    } else if (msg->getArrivalGateId() == taskmanagerin) {
        handleTaskAck(msg);
    } else if (msg->getArrivalGateId() == -1) {
        throw cRuntimeError(
                "No self message and no gateID?? Check configuration.");
    } else {
        throw cRuntimeError(
                "Unknown gateID?? Check configuration or override handleMessage().");
    }
}

void CarApplMAOCO::onWSA(WaveServiceAdvertisment* wsa) {

}

void CarApplMAOCO::finish() {
    BaseWaveApplLayer::finish();


    recordScalar("TaskCompeleteRate", (receiveTasksCount*1.0/generateTasksCount*1.0)*100.0);
    recordScalar("chosen RSU way", chosenRSUway);
    emit(taskCompeleteRate, (receiveTasksCount*1.0/generateTasksCount*1.0)*100.0);

    std::ofstream OsWrite("C:/Users/cojims/Desktop/papers/paper2/MAOCOdata/resultofVchange.csv",std::ofstream::app);
    OsWrite<<myId<<',';
    OsWrite<<ReV<<',';
    double summ = std::accumulate(std::begin(Recosts), std::end(Recosts), 0.0);
    double mean =  summ / Recosts.size();
    OsWrite<<mean<<',';
    summ = std::accumulate(std::begin(Redelays), std::end(Redelays), 0.0);
    mean =  summ / Redelays.size();
    OsWrite<<mean<<',';
    summ = std::accumulate(std::begin(Reutivalues), std::end(Reutivalues), 0.0);
    mean =  summ / Reutivalues.size();
    OsWrite<<mean<<std::endl;
    OsWrite.close();


//    OsWrite<<recvBuf;
//    OsWrite<<RecF;
//    OsWrite<<std::endl;
//    OsWrite.close();

}


void CarApplMAOCO::onWSM(WaveShortMessage* wsm) {

    if (wsm->getPsid() == PSIDTSK) {


        TaskRequest *tsk = dynamic_cast<TaskRequest*>(wsm->getObject(
                "taskRequest"));
        ASSERT(tsk);
        if (tsk->getTaskownerid() == getId()) {
            applend = getSimulation()->getSimTime();
            appldelay = applend - tsk->getGenerateTime();
            delays.record(appldelay.dbl());
            emit(applDelaysSignal, appldelay.dbl());
            TaskCosts.record(tsk->getTotalCost());

            chosenK.record(tsk->getComputationGain());
            TSKevaluateDelays.record(tsk->getEvaluateTime());
            TSKdelayinserver.record(tsk->getTimefromserver()-tsk->getTimetoserver());
            utilities.record(decider->getP(appldelay.dbl(),tsk->getTotalCost(),tsk->getComputationGain()));


            Recosts.push_back(tsk->getTotalCost());
            Redelays.push_back(appldelay.dbl());
            Reutivalues.push_back(decider->getP(appldelay.dbl(),tsk->getTotalCost(),tsk->getComputationGain()));

            receiveTasksCount++;
            offLoadTasksCount++;
        }

    } else if (wsm->getPsid() == BROADCASTINFO) {


/*****************MAOCAO related*****************************/

//        RSUxs.record(wsm->getTestO());
//        RSUxs.record(25.0);


        RSUposition=dynamic_cast<WSMwithSignal*>(wsm)->getSelfPosition();
        RSUxs.record(RSUposition.x);
        RSUys.record(RSUposition.y);
        connectedRSUID=wsm->getSenderAddress();

        auto pos=RSUlocations.find(connectedRSUID);
        if(pos==RSUlocations.end()? 1:0){

            RSUlocations.insert(std::pair<int,Coord>(connectedRSUID,RSUposition));
//            decider->addQ();
        }

        auto pla=players.find(connectedRSUID);
        if(pla==players.end()? 1:0){
            players.insert(std::pair<int,Player*>(connectedRSUID,new Player(alaphaj,betaj,gamaj,geneRate,handleRate,tth,learnningRate)));
        }

        auto rsudr=RSUdatarates.find(connectedRSUID);
        if(rsudr==RSUdatarates.end()? 1:0){
            RSUdatarates.insert(std::pair<int,double>(connectedRSUID,dynamic_cast<WSMwithSignal*>(wsm)->getBasicDataRate()));
        }else{
            RSUdatarates[connectedRSUID]=dynamic_cast<WSMwithSignal*>(wsm)->getBasicDataRate();
        }



        auto pri=RSUprices.find(connectedRSUID);
        if(pri==RSUprices.end()? 1:0){
            RSUprices.insert(std::pair<int,double>(connectedRSUID,wsm->getRiE()));
        }else{
            RSUprices[connectedRSUID]=wsm->getRiE();
        }

        if(RSUlocations.size()==1){
            currentRSUID=connectedRSUID;
        }



/***********************************************************/

        rsudelay = getSimulation()->getSimTime().dbl()
                - wsm->getCreationTime().dbl();
        lastCheck= getSimulation()->getSimTime().dbl();
        //bubble("!!!!!!");

        rsuDelays.record(rsudelay);
        connectRsuId = wsm->getSenderAddress();
        emit(rsuDelaySignal, rsudelay);
        rsuID=wsm->getSenderModuleId();
        this->RiC=wsm->getRiC();
        this->RiE=wsm->getRiE();
        recePr.record(this->RiE);
    }

}

void CarApplMAOCO::offLoad(TaskRequest* tsk,int ConnectedRSUID,bool isMigration,bool isCloudExecution,int TargetM) {

    WSMwithSignal* wsm = new WSMwithSignal();
    puis.record(tsk->getQjE());
    populateWSM(wsm);
    wsm->setWsmData(mobility->getRoadId().c_str());
    wsm->addObject(tsk);
    wsm->setPsid(PSIDTSK);

/********************************MAOCO*******************************/
    wsm->setIsMigration(isMigration);
    wsm->setIsCloudExecution(isCloudExecution);
    wsm->setTargetMigrationID(TargetM);
    wsm->setConnectedRSUID(ConnectedRSUID);
    wsm->setRelatedSpeed(vehicleSpeed);

/********************************************************************/

//    wsm->setBitLength(5000 + normal(0.0, 100.0));
    wsm->setBitLength(5000 + normal(0.0, 100.0));
    ByteLengths.record(wsm->getByteLength());
    sendDown(wsm);
}

void CarApplMAOCO::handleTaskAck(cMessage* msg) {
    TaskRequest* tsk = dynamic_cast<TaskRequest*>(msg);
    applend = getSimulation()->getSimTime();
    appldelay = applend-tsk->getGenerateTime();
    delays.record(appldelay.dbl());
    emit(applDelaysSignal, appldelay.dbl());
    receiveTasksCount++;
    delete tsk;
}

void CarApplMAOCO::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

    lastDroveAt = simTime();

}
