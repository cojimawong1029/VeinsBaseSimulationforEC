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

import org.car2x.veins.modules.edgecomputing.EdgeServerMAOCO;
import org.car2x.veins.modules.edgecomputing.EdgeServer;
import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.modules.edgecomputing.CloudServerMAOCO;
import org.car2x.veins.modules.edgecomputing.EvaluateModule.EvaluateModule;
import org.car2x.veins.modules.edgecomputing.MatLabInterface.MatLabCommand;

network RSUExampleScenarioStraight extends Scenario
{
    @display("bgb=1500,500");
    //@display("bgi=maps/grid,s;bgb=2500,2500;bgu=,");
    submodules:
        rsu1: EdgeServerMAOCO {
            @display("p=1,50;is=l");
        }
        rsu2: EdgeServerMAOCO {
            @display("p=125,50;is=l");
        }
        rsu3: EdgeServerMAOCO {
            @display("p=250,50;is=l");
        }
        rsu4: EdgeServerMAOCO {
            @display("p=375,50;is=l");
        }
        rsu5: EdgeServerMAOCO {
            @display("p=500,50;is=l");
        }

        rsu6: EdgeServerMAOCO {
            @display("p=625,50;is=l");
        }
        rsu7: EdgeServerMAOCO {
            @display("p=750,50;is=l");
        }
        rsu8: EdgeServerMAOCO {
            @display("p=875,50;is=l");
        }
        rsu9: EdgeServerMAOCO {
            @display("p=999,50;is=l");
        }

        cloud: CloudServerMAOCO {
            @display("p=1053,151;is=l");
        }

        evaluate: EvaluateModule {

            @display("p=30,68");
        }

        matlabCo: MatLabCommand {
            @display("p=28.73,167.31");
        }





    connections:
        rsu1.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu2.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu3.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu4.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu5.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu6.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu7.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu8.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;
        rsu9.cloudserverOut --> {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } --> cloud.connectsIn++;


        rsu1.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu2.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu3.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu4.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu5.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu6.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu7.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu8.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
        rsu9.cloudserverIn <-- {  delay = 0ms; datarate = 500000Mbps;@display("ls=,00"); } <-- cloud.connectsOut++;
}
