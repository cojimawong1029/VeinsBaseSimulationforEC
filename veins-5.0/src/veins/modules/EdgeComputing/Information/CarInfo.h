/*
 * CarInfo.h
 *
 *  Created on: 2020Äê4ÔÂ29ÈÕ
 *      Author: cojims
 */

#ifndef SRC_VEINS_MODULES_EDGECOMPUTING_INFORMATION_CARINFO_H_
#define SRC_VEINS_MODULES_EDGECOMPUTING_INFORMATION_CARINFO_H_

class CarInfo{
    protected:
    double vehicleSpeed;
    double lanePosition;
    std::string roadId;
    int roadIndex;
    int carId;

    public:
    CarInfo(){

    }

    CarInfo(double speed, double lanePosition,std::string roadID,int roadIndex,int carId){
        this->vehicleSpeed=speed;
        this->lanePosition=lanePosition;
        this->roadId=roadID;
        this->roadIndex=roadIndex;
        this->carId=carId;
    }

    void setCarId(int id){
        this->carId=id;
    }

    int getCarId(){
        return this->carId;
    }


    int getRoadIndex(){
        return this->roadIndex;
    }

    void setRoadIndex(int roadIndex){
        this->roadIndex=roadIndex;
    }

    void setVehicleSpeed(double Speed){
        this->vehicleSpeed=Speed;
    }

    void setLanePosition(double lanePosition){
        this->lanePosition=lanePosition;
    }

    void setRoadId(std::string roadId){
        this->roadId=roadId;
    }

    double getVehicleSpeed(){
        return this->vehicleSpeed;
    }

    double getLanePosition(){
        return this->lanePosition;
    }

    std::string getRoadId(){
        return this->roadId;
    }
};



#endif /* SRC_VEINS_MODULES_EDGECOMPUTING_INFORMATION_CARINFO_H_ */
