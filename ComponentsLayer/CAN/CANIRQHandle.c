//
// Created by bddwy on 2021/1/28.
//

#include "includes.h"
#include "CANCommunication.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CANCommunicationRxIRQHandler(hcan);
}
