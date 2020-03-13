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

import org.car2x.veins.modules.edgecomputing.EdgeServer;
import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.modules.edgecomputing.CloudServer;
import org.car2x.veins.modules.edgecomputing.EvaluateModule.EvaluateModule;

network RSUExampleScenarioStraight extends Scenario
{
    //@display("bgi=maps/grid,s;bgb=2500,2500;bgu=,");
    submodules:
        rsu1: EdgeServer {
            @display("p=1,50;is=l");
        }
        rsu2: EdgeServer {
            @display("p=250,50;is=l");
        }
        rsu3: EdgeServer {
            @display("p=500,50;is=l");
        }
        rsu4: EdgeServer {
            @display("p=750,50;is=l");
        }
        rsu5: EdgeServer {
            @display("p=999,50;is=l");
        }

        cloud: CloudServer {
            @display("p=1053,151;is=l");
        }

        evaluate: EvaluateModule {

            @display("p=30,68");
        }
        
        



    connections:
        rsu1.cloudserverOut --> {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu2.cloudserverOut --> {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu3.cloudserverOut --> {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu4.cloudserverOut --> {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu5.cloudserverOut --> {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } --> cloud.connectsIn++;

        rsu1.cloudserverIn <-- {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu2.cloudserverIn <-- {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu3.cloudserverIn <-- {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu4.cloudserverIn <-- {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu5.cloudserverIn <-- {  delay = 100ms; datarate = 100Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
}
