//
// Created by bddwy on 2021/1/26.
//

#include "CANCommunication.h"

__weak CAN_HandleTypeDef hcan1{nullptr}, hcan2{nullptr};

/**
 * @brief Constructor of CAN basic communication tasks, no matter hcan is valid or not
 * @param hcan CAN handle
 * @param TestMode CAN Mode, defalut = NORMAL
 * @return HAL_Status
 */
CANCommunication::CANCommunication(CAN_HandleTypeDef *hcan, CANCommunicationTestMode TestMode)
        : hcan(hcan) {

    if (hcan->Instance != nullptr){
        CANCommunicationInit(TestMode);
    }
    else{

    }
}

/**
 * @brief CAN filter config
 * @return HAL_Status
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
 * @param TestMode CAN Mode, default = NORMAL
 * @return HAL_Status
 */
HAL_StatusTypeDef CANCommunication::CANCommunicationInit(CANCommunicationTestMode TestMode) {
    if(hcan == &hcan1) MX_CAN1_Init();
    if(hcan == &hcan2) MX_CAN2_Init();

    TryHALConfig(CANCommunicationFilterConfig());
    TryHALConfig(HAL_CAN_Start(hcan));
    ReliableHALExec(HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING), 10);
    ReliableHALExec(HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY), 10);

    //everRx = 0;
    return HAL_OK;
}

/**
 * @brief Change CAN mode
 * @param TestMode CAN Mode
 * @return HAL_Status
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

    /** todo
    hcan->Init.Mode = static_cast<uint32_t>(TestMode);
    WRITE_REG(hcan->Instance->BTR, (uint32_t)(hcan->Init.Mode           |
                                              hcan->Init.SyncJumpWidth  |
                                              hcan->Init.TimeSeg1       |
                                              hcan->Init.TimeSeg2       |
                                              (hcan->Init.Prescaler - 1U)));
                                              */
}

/**
 * @brief Tx handler of CAN communication
 * @param TxHeader ID, IDE, RTR, DLC of the Tx message
 * @param TxMsg Message content, according to CAN specification, the size must be limited to 8 Bytes
 * @param MailBox Can be chosen from CAN_TX_MAILBOX0, CAN_TX_MAILBOX1 and CAN_TX_MAILBOX2, default = CAN_TX_MAILBOX0
 * @return HAL_Status
 */
HAL_StatusTypeDef CANCommunication::CANCommunicationTxHandler(CAN_TxHeaderTypeDef TxHeader, uint8_t *TxMsg, uint32_t MailBox) {
    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0) {
        return HAL_CAN_AddTxMessage(hcan, &TxHeader, TxMsg, (uint32_t *) MailBox);
    }

    return HAL_BUSY;
}

HAL_StatusTypeDef CANCommunication::CANCommunicationRxHandler() {
    return HAL_BUSY;
}

extern "C"{
    void CANCommunicationRxIRQHandler(CAN_HandleTypeDef *hcan) {
        if(hcan == &hcan1){
            CANCommunication::CAN1Communication.CANCommunicationRxHandler();
        }
        if(hcan == &hcan2){
            CANCommunication::CAN2Communication.CANCommunicationRxHandler();
        }
    }
}

CANCommunication CANCommunication::CAN1Communication(&hcan1);
CANCommunication CANCommunication::CAN2Communication(&hcan2);
