
#ifndef CYLINDERLOOP_CANREGISTRATION_H
#define CYLINDERLOOP_CANREGISTRATION_H

#include "CANCommunication.h"

enum CANFactoryStateTypeDef{
    CAN_FACTORY_ERROR = 0,
    CAN_FACTORY_OK,
};

class CANFactory{
public:
    CANFactory(){};

private:
    CAN_HandleTypeDef* hcan;
};

class CANRxFactory:public CANFactory{
public:
    CANRxFactory();

    CANFactoryStateTypeDef CANRxFactoryRegistrate();

    static CANRxFactory CAN1RxFactory;
private:
};

#endif //CYLINDERLOOP_CANREGISTRATION_H
