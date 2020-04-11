//
// Copyright (C) 2018 Gurjashan Pannu <pannu@ccs-labs.org>
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



packet TaskRequest {
    // The corresponding WSM's tree id
   
    double mi;
    string taskmodule="common";
    double stme;
    double dyme;
    int taskownerid;
    int tasksendid;
    int VmId=-1;
    int DataByteLength;
    int cloudGate;
    double lastTime;
   
    
//    int IsMigration=0;
//    int MigrationID;
//    int IsCloud=0;
    
    //the data in the procotol
    
    
    
    double deadline;
    int priority;
    
    int psid=12;
    bool batchPFlag=false;
    double geneRate;
    double tTh;
    double alapha;
    double gama;
    double beta;
    
    double qj;
    double riC=-1;
    double riE=-1;
    double qjC=1;
    double qjE=1;
    bool direct=false;
    
    int exe=0;
    
    int purchexe=0;
    
    //MAOCO-related
    bool isMigration=0;
	int TargetMigrationID;
	bool isCloudExecution=0;
    bool CloudToEdgeExe=0;
    
    
    

    
    // Access category on which the AckTimer is set
    //int ac = -1;
}
