//
// Created by bddwy on 2021/1/26.
//

#include "CANCommunication.h"

__weak CAN_HandleTypeDef hcan1{nullptr}, hcan2{nullptr};

CANCommunication::CANCommunication(CAN_HandleTypeDef *hcan, CANCommunicationTestMode TestMode)
        : hcan(hcan) {

    if(hcan->Instance == nullptr){

    }
    else{
        CANCommunicationInit(TestMode);
    }
}

HAL_StatusTypeDef CANCommunication::CANCommunicationInit(CANCommunicationTestMode TestMode) {
    if(hcan == &hcan1) MX_CAN1_Init();
    if(hcan == &hcan2) MX_CAN2_Init();

    HAL_CAN_Start(hcan);
    ReliableHALExec(HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING), 10);
    ReliableHALExec(HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY), 10);

    //everRx = 0;
}

HAL_StatusTypeDef CANCommunication::CANCommunicationChangeTestMode(CANCommunicationTestMode TestMode) {
    if(hcan->Init.Mode != static_cast<uint32_t>(TestMode)){
        hcan->Init.Mode = static_cast<uint32_t>(TestMode);
        return HAL_CAN_Init(hcan);
    }
    else{
        return HAL_OK;
    }

    /*
    hcan->Init.Mode = static_cast<uint32_t>(TestMode);
    WRITE_REG(hcan->Instance->BTR, (uint32_t)(hcan->Init.Mode           |
                                              hcan->Init.SyncJumpWidth  |
                                              hcan->Init.TimeSeg1       |
                                              hcan->Init.TimeSeg2       |
                                              (hcan->Init.Prescaler - 1U)));
                                              */
}

CANCommunication CANCommunication::CAN1Communication(&hcan1);
CANCommunication CANCommunication::CAN2Communication(&hcan2);
