//
// Created by bddwy on 2021/1/28.
//

#include "includes.h"

extern void CANCommunicationRxIRQHandler(CAN_HandleTypeDef *hcan);

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CANCommunicationRxIRQHandler(hcan);
}
