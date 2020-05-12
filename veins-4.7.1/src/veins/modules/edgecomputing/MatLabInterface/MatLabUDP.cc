/*
 * MatLabUDP.cc
 *
 *  Created on: 2020Äê4ÔÂ11ÈÕ
 *      Author: cojims
 */


#include "veins/modules/edgecomputing/MatLabInterface/MatLabUDP.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"

#include <string>
Define_Module(MatLabUDP);

#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#include <fstream>


void MatLabUDP::finish(){
    cSimpleModule::finish();
//    WORD wVersionRequested;
//    WSADATA wsaData;
//    int err;
//
//    wVersionRequested = MAKEWORD(1,1);
//
//    err = WSAStartup(wVersionRequested, &wsaData);
//    if(err != 0)
//    {
//      return;
//    }
//
//    if(LOBYTE(wsaData.wVersion) != 1 ||
//      HIBYTE(wsaData.wVersion) != 1)
//    {
//      WSACleanup();
//      return;
//    }
//
//    SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
//    sockaddr_in  addrSrv;
//    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
//    addrSrv.sin_family = AF_INET;
//    addrSrv.sin_port = htons(6969);
//    int len = sizeof(SOCKADDR);
//    char sendBuf[100]="SimulationOVER";
//    sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addrSrv,len);
//
//    closesocket(sockSrv);

}

void MatLabUDP::initialize(){
    datain=findGate("datain");
    resultout=findGate("resultout");
}

void MatLabUDP::handleMessage(cMessage* msg) {
    UDPData* udp=dynamic_cast<UDPData*>(msg);

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


    int len = sizeof(SOCKADDR);


    char recvBuf[100];
//    char sendBuf[100]="Hello Server";
//    char tempBuf[100];



    sendto(sockSrv,udp->getData(),strlen(udp->getData())+1,0,(SOCKADDR*)&addrSrv,len);
    //closesocket(sockSrv);
    int RecF=recvfrom(sockSrvRec,recvBuf,100,0,(SOCKADDR*)&addrSrvRec,&len);

    std::ofstream OsWrite("ot.txt",std::ofstream::app);
    OsWrite<<recvBuf;
    OsWrite<<RecF;
    OsWrite<<std::endl;
    OsWrite.close();



    closesocket(sockSrv);
    closesocket(sockSrvRec);


}
