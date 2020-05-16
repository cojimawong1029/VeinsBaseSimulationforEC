

package org.car2x.veins.modules.EdgeComputing.connectedApplications;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

simple EdgeApplSafe extends DemoBaseApplLayer
{
    parameters:
    	@class(veins::EdgeApplSafe);
    	@display("i=block/app2");
    	double broadcastInterval @unit("s")=default(1s);
		bool TCPMATLAB=default(false);
    gates:

        



        
}
