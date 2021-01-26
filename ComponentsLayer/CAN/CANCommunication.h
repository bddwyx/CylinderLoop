
#ifndef CYLINDERLOOP_CANCOMMUNICATION_H
#define CYLINDERLOOP_CANCOMMUNICATION_H

#include "includes.h"

enum CANCommunicationTestMode{
    NORMAL = CAN_MODE_NORMAL,
    LOOPBACK = CAN_MODE_LOOPBACK,
    SILENT = CAN_MODE_SILENT,
    SILENT_LOOPBACK = CAN_MODE_SILENT_LOOPBACK,
};

class CANCommunication{
public:
    explicit CANCommunication(CAN_HandleTypeDef *hcan, CANCommunicationTestMode TestMode = NORMAL);

    HAL_StatusTypeDef CANCommunicationFilterConfig();
    HAL_StatusTypeDef CANCommunicationInit(CANCommunicationTestMode TestMode = NORMAL);
    HAL_StatusTypeDef CANCommunicationChangeTestMode(CANCommunicationTestMode TestMode);

    static CANCommunication CAN1Communication, CAN2Communication;
private:
    CAN_HandleTypeDef* hcan;
    bool CANOnline[2] = {false};
};

#endif //CYLINDERLOOP_CANCOMMUNICATION_H
