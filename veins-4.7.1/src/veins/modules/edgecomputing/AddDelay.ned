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

simple AddDelay
{
    parameters:
        @class(AddDelay);
        @display("i=block/arrival");
        double dataTransRate=default(27);
        double rateLossRate=default(0.99);
        double rateChangeInterval @unit("s") = default(0.1s);

    gates:
        input downIn;
        input upIn;
        output downOut;
        output upOut;

}