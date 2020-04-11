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

import org.car2x.veins.modules.edgecomputing.MAOCO.*;

package org.car2x.veins.modules.edgecomputing;

module CloudServerMAOCO
{
    parameters:
        string cpuType = default("ComputerCloud");
        //string memoryType = default("Memory");
        @display("i=device/mainframe");
    gates:
        input connectsIn[];
        output connectsOut[];

    submodules:
        cpu: <cpuType> like org.car2x.veins.modules.edgecomputing.IComputeResource {
            @display("p=235,94");
        }

        //        memory: <memoryType> like IMemory {
        //            @display("p=270,167");
        //        }
        cloudappl: CloudApplMAOCO {
            parameters:

                @display("p=166,95");
            gates:
                connect[sizeof(connectsIn)];
        }
        queue[sizeof(connectsIn)]: TxQueue;
    connections allowunconnected:
        //cpu.memory <--> memory.cpu;
        cloudappl.cpu <--> cpu.applLayer;

        for i=0..(sizeof(connectsIn)-1) {
            connectsIn[i] --> queue[i].downIn;
            connectsOut[i] <-- queue[i].downOut;
            queue[i].up <--> cloudappl.connect[i];
        }

}
