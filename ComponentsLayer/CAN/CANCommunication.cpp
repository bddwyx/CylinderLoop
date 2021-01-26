//
// Created by bddwy on 2021/1/26.
//

#include "CANCommunication.h"

__weak CAN_HandleTypeDef hcan1{nullptr}, hcan2{nullptr};

/**
 * @brief Constructor of CAN basic communication tasks, no matter hcan is valid or not
 * @param hcan CAN handle
 * @param TestMode CAN Mode
 */
CANCommunication::CANCommunication(CAN_HandleTypeDef *hcan, CANCommunicationTestMode TestMode)
        : hcan(hcan) {

    if(hcan->Instance == nullptr){

    }
    else{
        CANCommunicationInit(TestMode);
    }
}

/**
 * @brief CAN filter config
 */
HAL_StatusTypeDef CANCommunication::CANCommunicationFilterConfig() {
    CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.SlaveStartFilterBank = 0;

    return HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
}

/**
 * @brief Initialize valid CAN communication
 * @param TestMode CAN Mode
 */
HAL_StatusTypeDef CANCommunication::CANCommunicationInit(CANCommunicationTestMode TestMode) {
    if(hcan == &hcan1) MX_CAN1_Init();
    if(hcan == &hcan2) MX_CAN2_Init();

    TryHALConfig(CANCommunicationFilterConfig());
    TryHALConfig(HAL_CAN_Start(hcan));
    ReliableHALExec(HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING), 10);
    ReliableHALExec(HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY), 10);

    //everRx = 0;
}

/**
 * @brief Change CAN mode
 * @param TestMode CAN Mode
 * @todo Operate by register
 */
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
