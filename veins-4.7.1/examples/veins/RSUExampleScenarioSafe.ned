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

import org.car2x.veins.modules.edgecomputing.EdgeServerSafe;
import org.car2x.veins.modules.edgecomputing.EdgeServer;
import org.car2x.veins.nodes.Scenario;
import org.car2x.veins.modules.edgecomputing.CloudServerMAOCO;
import org.car2x.veins.modules.edgecomputing.EvaluateModule.EvaluateModule;

network RSUExampleScenarioSafe extends Scenario
{
    @display("bgb=500,500");
    //@display("bgi=maps/grid,s;bgb=2500,2500;bgu=,");
    submodules:
        rsu1: EdgeServerSafe {
            @display("p=200,200;is=l");
        }


        evaluate: EvaluateModule {

            @display("p=30,68");
        }





    connections:

}