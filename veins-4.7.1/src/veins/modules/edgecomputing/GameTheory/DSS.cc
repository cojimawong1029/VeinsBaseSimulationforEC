/*
 * DSS.cc
 *
 *  Created on: 2019Äê8ÔÂ29ÈÕ
 *      Author: xiaow
 */
#include <math.h>
#include <veins/modules/edgecomputing/GameTheory/DSS.h>

void DSS::setWeight(double alapha, double beta, double gama) {
    this->alapha = alapha;
    this->beta = beta;
    this->gama = gama;
}

double DSS::purchaseCRB(double geneRate, double handleRate, double unitRate) {
    return (1 / (handleRate * pow((unitRate * beta / (gama*handleRate*geneRate)), 0.5)))
            + geneRate / handleRate;
}

void DSS::setGene(double geneRate) {
    this->geneRate = geneRate;
}

double DSS::getUtility(double geneRate, double handleRate, double unitRate,
        double getlink, double dataRate, double datasize) {
    return alapha * geneRate
            - beta * purchaseCRB(geneRate, handleRate, unitRate) * unitRate
            - gama * geneRate
                    * gettj(unitRate, handleRate, geneRate, getlink, dataRate,
                            datasize);
}
double DSS::gettj(double unitRate, double handleRate, double geneRate,
        double getlink, double dataRate, double datasize) {
    return (1/(purchaseCRB(geneRate, handleRate, unitRate)
                    * handleRate - geneRate)) + getlink + datasize / dataRate;
}


double DSS::getPostUtility(double geneRate,double handleRate,double unitRate,double delay,double numCB){
    return alapha*geneRate-beta*numCB*unitRate-gama*geneRate*delay;
}

double DSS::getsUtility(double geneRate, double handleRate, double unitRate,
        double getlink, double dataRate, double datasize) {
    return alapha * geneRate
            - gama * geneRate
                    * (1/ (1 * handleRate - geneRate) + getlink + datasize / dataRate);
}
