//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
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
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;

simple CarApplGame extends BaseWaveApplLayer
{
    parameters:
        @class(CarApplGameC);
        @display("i=block/app2");
        int TaskCount = default(5000);


        @signal[applDelays](type="double");
        @signal[rsuDelay](type="double");
        @signal[applCompeleteRate](type="double");
        //    double TaskStart @unit("s") = default(5s);  
        //    volatile double TaskDuration @unit("s") = default(1s);
        //    volatile double TaskInterval @unit("s") = default(1s);
        double TaskStart @unit("s") = default(uniform(0s,5s));
        //volatile double TaskDuration @unit("s") = default(0.05s);
        //volatile double TaskInterval @unit("s") = default(0.05s);
        double TaskGeneInterval @unit("s")=default(0.1s);
        double bingInerval @unit("s") = default(1s);
        double pingInterval @unit("s") = default(1s);
        double alaphaj=default(1);
        double betaj=default(0.5);
        double gamaj=default(50.0);
        double geneRate=default(10);




    gates:
        //input TaskGenerator@directIn;
        //inout cpu;
        //inout memory;
        inout taskManager;
}
