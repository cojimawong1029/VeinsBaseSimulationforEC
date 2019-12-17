/*
 * DSO.h
 *
 *  Created on: 2019Äê8ÔÂ29ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_DSO_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_DSO_H_

#include <map>

struct DSSData {
    double geneRate;
    double numCRB;
    double alapha;
    double beta;
    double gama;
    double tTh;
};

class DSO {
public:
    double calUnitRate(double geneRate, double tTh, double handleRate,double beta, double gama,double eq,double maxp=0.5);
//    void insertDSSData(int DSSID, double geneRate, double numCRB, double alapha, double beta,double gama, double tTh);
//    void insertDSSData(int DSSID, DSSData data);
//    std::map<int, double> calRs();


    double littleprice(double geneRate,double beta, double gama,double handleRate,double eq);

    double maxprice(double geneRate, double tTh, double handleRate,double beta, double gama);



private:

//    std::map<int, DSSData> DssMaps;
//    std::map<int, double> DssRs;
//   double handleRate;

};

#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_DSO_H_ */
