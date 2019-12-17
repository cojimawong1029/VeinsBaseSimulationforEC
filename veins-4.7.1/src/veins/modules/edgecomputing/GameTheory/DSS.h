/*
 * DSS.h
 *
 *  Created on: 2019Äê8ÔÂ29ÈÕ
 *      Author: xiaow
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_DSS_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_DSS_H_

class DSS {
public:
    double purchaseCRB(double geneRate, double handleRate, double unitRate);
    void setWeight(double alapha, double beta, double gama);
    double getUtility(double geneRate, double handleRate, double unitRate,
            double getlink, double dataRate, double datasize);
    double gettj(double UnitRate,double handleRate, double geneRate, double getlink,
            double dataRate, double datasize);
    void setGene(double GeneRate);
    double getsUtility(double geneRate, double handleRate, double unitRate,
            double getlink, double dataRate, double datasize);

    double getPostUtility(double geneRate,double handleRate,double unitRate,double delay,double numCB);

private:
    double alapha, beta, gama;
    double geneRate;

};

#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_DSS_H_ */
