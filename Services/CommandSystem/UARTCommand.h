//
// Created by bddwy on 2021/1/28.
//

#ifndef CYLINDERLOOP_UARTCOMMAND_H
#define CYLINDERLOOP_UARTCOMMAND_H

#include "includes.h"

class UARTCommand{
public:
    UARTCommand();

    HAL_StatusTypeDef UARTCommandInit();
    HAL_StatusTypeDef UARTCommandReset();
private:
    UART_HandleTypeDef huart;
};

extern "C"{
    void UARTCommandRecv();
}

#endif //CYLINDERLOOP_UARTCOMMAND_H
