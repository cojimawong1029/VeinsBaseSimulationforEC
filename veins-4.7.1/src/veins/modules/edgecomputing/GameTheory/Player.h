/*
 * Player.h
 *
 *  Created on: 2019Äê11ÔÂ26ÈÕ
 *      Author: cojims
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_PLAYER_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_PLAYER_H_

class Player{


private:
    double alapha,beta,gamma,lambda,mu,tth;
    double qp_1{0},qp_2{1},up_1{100},up_2{100};
    double sr;


public:
    Player(double alapha,double beta,double gamma,double lambda,double mu,double tth,double sr);

    double getUtilityPre(double qi, double data, double datarate,double pi,double getlink);
    double getEW(double qi);
    double getEWwithK(double qi,double K);
    double getED(double qi,double data, double datarate,double getlink);

    double getUtilityPost(double qi,double pi,double texe);

    double iterQ();

    void addPreDecision(double qi,double ui);

    double getAlapha(){
        return this->alapha;
    }
    double getBeta(){
        return this->beta;
    }
    double getGamma(){
        return this->gamma;
    }
    double getLambda(){
        return this->lambda;
    }
    double getMu(){
        return this->mu;
    }
    double getTth(){
        return this->tth;
    }
    void setAlapha(double alapha){
        this->alapha=alapha;
    }
    void setBeta(double beta){
        this->beta=beta;
    }
    void setGamma(double gamma){
        this->gamma=gamma;
    }
    void setLambda(double lambda){
        this->lambda=lambda;
    }
    void setMu(double mu){
        this->mu=mu;
    }
    void setTth(double tth){
        this->tth=tth;
    }

};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_GAMETHEORY_PLAYER_H_ */
