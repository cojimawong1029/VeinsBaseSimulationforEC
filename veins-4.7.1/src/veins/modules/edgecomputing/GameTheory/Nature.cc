/*
 * Nature.cc
 *
 *  Created on: 2019Äê11ÔÂ26ÈÕ
 *      Author: cojims
 */



#include <math.h>
#include <veins/modules/edgecomputing/GameTheory/Nature.h>

Nature::Nature(double x,double y,double tau){
    this->x=x;
    this->y=y;
    this->tau=tau;
}

double Nature::getPrice(double Q){
    return x+y*pow(Q,tau);
}


