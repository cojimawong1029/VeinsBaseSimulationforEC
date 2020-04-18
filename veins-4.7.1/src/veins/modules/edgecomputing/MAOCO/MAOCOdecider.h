/*
 * MAOCOdecider.h
 *
 *  Created on: 2020Äê4ÔÂ13ÈÕ
 *      Author: cojims
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_MAOCO_MAOCODECIDER_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_MAOCO_MAOCODECIDER_H_

#include<deque>
#include<vector>
#include<math.h>
#include "veins/modules/edgecomputing/GameTheory/Player.h"

class MAOCOdecider{
private:
    double alpha,beta,gamma;
    double V;
    std::vector<double> Q;

    virtual double SumQandy(std::vector<double> y);

public:
    virtual void setoneQ(int index,double num){
        this->Q[index]=num;
    }


    virtual double getP(double delay, double cost, double gain);
    virtual void setWeight(double alpha,double beta,double gamma);
    virtual void setV(double V);
    virtual void addQ(){
        Q.push_back(0);
        //Q.push_back(0);
    }
    virtual int getQsize(){
        return Q.size();
    }

    virtual double getQbyIndex(int index){
        return Q[index-1];
    }

    virtual void removeQ(int number){
        auto it=Q.begin();
        for(int i=1;i<number;i++)
            it++;
        Q.erase(it);

    }

    virtual void updateQ(std::vector<double> y);
    virtual double getLyapnovValue(double P,std::vector<double> y);
    virtual double findBestKSimple(double delay,double costRate){
        return tan((beta*costRate+alpha*delay)/gamma);
    }

    virtual double findBestKbyItera(double delay,double costRate);
    virtual double findBestKbyIterawithPlayer(double qi,double cost,Player* dss);





};





#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_MAOCO_MAOCODECIDER_H_ */
