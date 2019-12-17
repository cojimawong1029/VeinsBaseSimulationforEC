/*
 * DSO.cc
 *
 *  Created on: 2019Äê8ÔÂ29ÈÕ
 *      Author: xiaow
 */

#include <veins/modules/edgecomputing/GameTheory/DSO.h>
#include <math.h>

double DSO::calUnitRate(double geneRate, double tTh, double handleRate,double beta, double gama,double eq, double maxp) {
    double i,j;
    i=littleprice(geneRate,beta,gama,handleRate,eq);
    j=maxprice(geneRate,tTh,handleRate,beta,gama);
//    if (i>j)
//        return j;0
//    else return i;
//    return maxp*j+(1-maxp)*i;

    if (i<j)
//        return j;
        return maxp*j+(1-maxp)*i;
    else
        return j;

}

//void DSO::insertDSSData(int DSSID, double geneRate, double numCRB,double alapha, double beta, double gama, double tTh) {
//    DSSData data = { geneRate, numCRB, alapha, beta, gama, tTh };
//    insertDSSData(DSSID, data);
//}
//void DSO::insertDSSData(int DSSID, DSSData data) {k
//    DssMaps.insert(std::pair<int, DSSData>(DSSID, data));
//}


double DSO::littleprice(double geneRate,double beta, double gama,double handleRate,double eq){
    return pow(((1/(eq-geneRate/handleRate))/handleRate),2)*((gama*geneRate*handleRate)/beta);
}

double DSO::maxprice(double geneRate, double tTh, double handleRate,double beta, double gama){
    return (gama / beta) * pow(((handleRate * tTh - geneRate) / geneRate), 2);
}



//std::map<int, double> DSO::calRs(){
//    for(auto it=DssMaps.begin();it!=DssMaps.end();it++){
//        DSSData data=it->second;
//        double Ri=calUnitRate(data.geneRate,data.tTh,handleRate,data.beta,data.gama);
//        DssRs.insert(std::pair<int,double>(it->first,Ri));
//    }
//    return DssRs;
//}
