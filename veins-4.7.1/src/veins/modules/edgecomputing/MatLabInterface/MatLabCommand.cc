/*
 * MatLabCommand.cc
 *
 *  Created on: 2020Äê4ÔÂ11ÈÕ
 *      Author: cojims
 */


#include "veins/modules/edgecomputing/MatLabInterface/MatLabCommand.h"
#include "veins/modules/edgecomputing/EdgeMessage/TaskRequest_m.h"

#include <string>
Define_Module(MatLabCommand);

#include <Winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
#include <fstream>


void MatLabCommand::finish(){
//    cSimpleModule::finish();
    if(enableUDP){
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
        sockaddr_in  addrSrv;
        addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_port = htons(6969);
        int len = sizeof(SOCKADDR);
        char sendBuf[100]="SimulationOVER";
        sendto(sockSrv,sendBuf,strlen(sendBuf),0,(SOCKADDR*)&addrSrv,len);

        closesocket(sockSrv);
    }


}

void MatLabCommand::initialize(){
    enableUDP=par("enableUDP");
}
