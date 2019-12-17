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
simple CloudAppl{
    parameters:
        @display("i=block/app2");
        @class(CloudApplGame);
        @signal[test](type="double");
        @signal[price](type="double");
        double sendprice @unit("s") = default(0.1s);
        
        
        double alaphaj=default(1);
        double betaj=default(0.5);
        double gamaj=default(50.0);
        double geneRate=default(10);
    
    gates:
        inout cpu;
    
        inout connect[];
}

