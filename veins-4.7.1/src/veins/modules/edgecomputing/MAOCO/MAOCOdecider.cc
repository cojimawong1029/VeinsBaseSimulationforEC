/*
 * MAOCOdecider.cc
 *
 *  Created on: 2020Äê4ÔÂ13ÈÕ
 *      Author: cojims
 */

#include <veins/modules/edgecomputing/MAOCO/MAOCOdecider.h>
#include <math.h>


double MAOCOdecider::getP(double delay, double cost,double gain){
    return -alpha*delay-beta*cost+gamma*gain;
}

void MAOCOdecider::setWeight(double alpha,double beta,double gamma){
    this->alpha=alpha;
    this->beta=beta;
    this->gamma=gamma;
}


void MAOCOdecider::updateQ(std::vector<double> y){
    for(int i=0;i<Q.size();i++){
        Q[i]=std::max(Q[i]+y[i],0.0);
    }
}


double MAOCOdecider::getLyapnovValue(double P,std::vector<double> y){
    return V*P+SumQandy(y);
}

double MAOCOdecider::SumQandy(std::vector<double> y){
    double sum=0;
    for(int i=0;i<Q.size();i++){
        sum+=Q[i]*y[i];
    }
    return sum;
}


void MAOCOdecider::setV(double V){
    this->V=V;
}

double MAOCOdecider::findBestKbyItera(double delay,double costRate){
    double K1=1,K2=1.5;
    double W1,W2;
    double tem;
    for(int i=0;i<25;i++){
        if(K1<=1)K1=1;
        if(K2<=1)K2=1.5;
        if(K1>=10)K1=9.8;
        if(K2>=10)K2=10;
        W1=getP(K1*delay,costRate,K1);
        W2=getP(K2*delay,costRate,K2);

        if(rand()/32767.0<0.8){
            tem=K2;
            K2=K2+0.0005*(W2-W1)*(K2-K1);
            K1=tem;
        }else{
            K2=K2+0.0005*(K2-K1)*(W2-W1);
            K1=1+(rand()/32767.0)*48;
        }
    }
    if(K2<=1)K2=1.0;
    if(K2>=10)K2=10;

    return K2;
}

double MAOCOdecider::findBestKbyIterawithPlayer(double qi,double cost,Player* dss){
    double K1=1,K2=1.5;
        double W1,W2;
        double tem;
        double delay1, delay2;
        for(int i=0;i<25;i++){
            if(K1<=1)K1=1;
            if(K2<=1)K2=1.5;
            if(K1>=10)K1=9.8;
            if(K2>=10)K2=10;
            delay1=dss->getEWwithK(qi,K1);
            delay1=dss->getEWwithK(qi,K2);
            W1=getP(delay1,cost,K1);
            W2=getP(delay2,cost,K2);

            if(rand()/32767.0<0.8){
                tem=K2;
                K2=K2+0.0005*(W2-W1)*(K2-K1);
                K1=tem;
            }else{
                K2=K2+0.0005*(K2-K1)*(W2-W1);
                K1=1+(rand()/32767.0)*48;
            }
        }
        if(K2<=1)K2=1.0;
        if(K2>=10)K2=10;

        return K2;
}

double MAOCOdecider::findBestKbyIterawithPlayerMath(double qi,double costRate,Player* dss){
    double tem1=(-costRate+gamma);
    double tem2=pow((qi*dss->getMu())/(-2*(tem1/-1/alpha-1/(2*qi*dss->getMu()))),0.5);
    double tem3=(qi*dss->getMu()-tem2)/dss->getLambda();
    return tem3;
}


