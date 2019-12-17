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

package org.car2x.veins.modules.edgecomputing;

simple ComputerOnBoard like IComputeResource
{
    parameters:

        double mips = default(5000000);
        double Eoff = default(0);
        double Eidle = default(162);
        double Ebusy = default(215);
        double ram = default(50000);
        int numofHost = default(2);
        double timeIndex = default(0.5);


        @signal[queueLength](type="long");
        @signal[bflag](type="long");
        @statistic[queueLength](title="queue length"; record=vector,timeavg,max; interpolationmode=sample-hold);
        @statistic[bflag](title="flags"; record=vector,timeavg,max; interpolationmode=sample-hold);

        @class(ComputerOnBoard);
        @display("i=device/cpu");

        string VMAllocation = default("FirstFit");

    gates:
        inout applLayer;

}
