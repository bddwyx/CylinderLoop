
#ifndef CYLINDERLOOP_CANREGISTRATION_H
#define CYLINDERLOOP_CANREGISTRATION_H

#include "CANCommunication.h"

enum CANFactoryStateTypeDef{
    CAN_FACTORY_ERROR = HAL_ERROR,
    CAN_FACTORY_OK = HAL_OK,
    CAN_FACTORY_BUSY = HAL_BUSY,
};

class CANFactory{
public:
    CANFactory();

protected:

private:

};

class CANRxFactory:public CANFactory{
public:
    CANRxFactory();

    CANFactoryStateTypeDef CANRxFactoryRegistrate();

    static CANRxFactory CAN12RxFactory;
private:
};

#endif //CYLINDERLOOP_CANREGISTRATION_H
