//
// Copyright (C) 2015-2018 Dominik Buse <dbuse@mail.uni-paderborn.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
package org.car2x.veins.modules.edgecomputing;

import org.car2x.veins.base.modules.*;
import org.car2x.veins.modules.nic.Nic80211p;
import org.car2x.veins.modules.edgecomputing.*;


module EdgeServer
{
    parameters:
        string applType = default("org.car2x.veins.modules.application.traci.MyVeinsApp"); //type of the application layer
        string nicType = default("Nic80211p"); // type of network interface card
        //string externalId; // TraCI server's id of the junction/tls managed by this module
        string cpuType = default("ComputerEdge");
        //string memoryType = default("Memory");
        @display("i=device/accesspoint");
    gates:
        input veinsradioIn; // gate for sendDirect
        input cloudserverIn;
        output cloudserverOut;
    submodules:
        appl: EdgeServerApplLayer {
            parameters:
                @display("p=60,90");
        }

        nic: <nicType> like org.car2x.veins.modules.nic.INic80211p {
            parameters:
                @display("p=60,206");
        }

        mobility: BaseMobility {
            parameters:
                @display("p=69,312;i=block/cogwheel");
        }


        cpu: <cpuType> like org.car2x.veins.modules.edgecomputing.IComputeResource {
            @display("p=191,89");
        }





        txQueue: TxQueue {
            @display("p=160,244");
        }
    connections:
        appl.cpu <--> cpu.applLayer;
        nic.upperLayerOut --> appl.lowerLayerIn;
        nic.upperLayerIn <-- appl.lowerLayerOut;
        nic.upperControlOut --> appl.lowerControlIn;
        nic.upperControlIn <-- appl.lowerControlOut;
        veinsradioIn --> nic.radioIn;
        txQueue.up <--> appl.cloud;

        txQueue.downOut --> cloudserverOut;
        cloudserverIn --> txQueue.downIn;
}
