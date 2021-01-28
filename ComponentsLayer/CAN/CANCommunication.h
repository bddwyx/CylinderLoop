
#ifndef CYLINDERLOOP_CANCOMMUNICATION_H
#define CYLINDERLOOP_CANCOMMUNICATION_H

#include "includes.h"

#ifdef __cplusplus
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

    HAL_StatusTypeDef CANCommunicationTxHandler(CAN_TxHeaderTypeDef TxHeader, uint8_t *TxMsg, uint32_t MailBox = CAN_TX_MAILBOX0);
    HAL_StatusTypeDef CANCommunicationRxHandler();

    static CANCommunication CAN1Communication, CAN2Communication;

private:
    CAN_HandleTypeDef* hcan;
    bool CANOnline[2] = {false};
};
#endif //_cplusplus

#ifdef __cplusplus
extern "C" {
#endif //_cplusplus

void CANCommunicationRxIRQHandler(CAN_HandleTypeDef *hcan);

#ifdef __cplusplus
}
#endif //_cplusplus

#endif //CYLINDERLOOP_CANCOMMUNICATION_H
