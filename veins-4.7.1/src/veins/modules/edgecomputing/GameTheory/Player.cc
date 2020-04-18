/*
 * Player.cc
 *
 *  Created on: 2019Äê11ÔÂ26ÈÕ
 *      Author: cojims
 */

#include <math.h>
#include <veins/modules/edgecomputing/GameTheory/Player.h>

Player::Player(double alapha,double beta,double gamma,double lambda,double mu,double tth,double sr){
    this->alapha=alapha;
    this->beta=beta;
    this->gamma=gamma;
    this->lambda=lambda;
    this->mu=mu;
    this->tth=tth;
    this->sr=sr;
}

double Player::getUtilityPre(double qi, double data, double datarate,double pi,double getlink){
    double texe=getED(qi,data,datarate,getlink);
    return alapha*pow((1/texe*tth),beta)-gamma*qi*pi;
}

double Player::getEWwithK(double qi,double K){
    double rou=this->lambda/qi/(this->mu/K);
    return (1/qi/(this->mu/K))+(this->lambda*(pow((1/qi/(this->mu/K)),2)))/(2*(1-rou));
}

double Player::getEW(double qi){
    double rou=this->lambda/qi/this->mu;
    return (1/qi/this->mu)+(this->lambda*(pow((1/qi/this->mu),2)))/(2*(1-rou));
}

double Player::getED(double qi,double data, double datarate,double getlink){
    return getEW(qi)+data/datarate+getlink;
}

double Player::getUtilityPost(double qi,double pi,double texe){

    return alapha*pow((1/texe*tth),beta)-gamma*qi*pi;
    //return (1/qi/this->mu)+(this->lambda*(pow((1/qi/this->mu),2)))/(2*(1-rou));
}

void Player::addPreDecision(double qi,double ui){
    this->qp_1=this->qp_2;
    this->up_1=this->up_2;
    this->qp_2=qi;
    this->up_2=ui;
}

double Player::iterQ(){
    double delta;
    if(qp_1==qp_2 && up_1==up_2){
        delta=0.1* qp_1;
    }else{
        if(qp_1==qp_2) qp_1=0.9*qp_2;
        delta=sr*(qp_1+qp_2)*0.5*((up_2-up_1)/(qp_2-qp_1));
//        if(q<1) q=1;
//        if(q>20) q=20;
//        return q;
    }

    if(delta>0.25){
        delta=0.25;
    }else if(delta<-0.25)
    {
        delta=-0.25;
    }
    double qi=0.5*(qp_1+qp_2)+delta;
    if(qi>30){
        qi=30;
    }else if(qi<1.001){
        qi=1.001;
    }
    return qi;
}

