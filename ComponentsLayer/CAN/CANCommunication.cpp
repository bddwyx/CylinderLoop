//
// Created by bddwy on 2021/1/26.
//

#include "CANCommunication.h"

void Error_Handler();

HAL_StatusTypeDef CANCommunicationInit(CAN_HandleTypeDef* hcan) {
    HAL_CAN_Start(hcan);
    HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    ReliableHALExec(HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY), 10);

    everRx = 0;
}
