//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package org.car2x.veins.modules.edgecomputing.MAOCO;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;
 simple EdgeServerApplLayerMAOCO extends BaseWaveApplLayer{
     parameters:
        double idlePower=default(50);
	    double busyPower=default(100);
	    @class(EdgeServerApplLayerMAOCO);
	    @display("i=block/app");
	    double pingInterval @unit("s") =default(1s);
	    double pingStart @unit("s") =default(0s);
	    
	    double alaphaj=default(1);
        double betaj=default(0.5);
        double gamaj=default(50.0);
        double geneRate=default(10);
        
        double lowprice=default(50);
        double changerate=default(2);
        double changeindictor=default(1.1);
        int channelUsed=default(174);

	    
	    //int headerLength @unit("bit");
	   // bool sendBeacons=default(false);
		gates:
		inout cpu;
		//inout memory;
		inout cloud;
		//inout cpu;
		//input lowerLayerIn; // from network layer
       // output lowerLayerOut; // to network layer
        //input lowerControlIn; // control from network layer
        //output lowerControlOut; // control to network layer

         
 }