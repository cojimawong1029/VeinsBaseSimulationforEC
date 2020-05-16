/*
 * MatLabCommand.h
 *
 *  Created on: 2020Äê4ÔÂ11ÈÕ
 *      Author: cojims
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_MatLabCommand_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_MatLabCommand_H_

#include <omnetpp.h>
using namespace omnetpp;
#include "veins/modules/messages/WaveShortMessage_m.h"
#include "veins/modules/edgecomputing/MAOCO/WSMwithSignal_m.h"
#include "veins/modules/edgecomputing/MatLabInterface/UDPData_m.h"

class MatLabCommand : public cSimpleModule
{
  private:


    SOCKET sockclient;
    struct sockaddr_in addr;
    bool enableUDP;



  public:


  protected:

  virtual void initialize() override;
  virtual void finish();





};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_MatLabCommand_H_ */
