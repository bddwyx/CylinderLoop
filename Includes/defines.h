//
// Created by bddwy on 2021/1/26.
//

#ifndef CYLINDERLOOP_DEFINES_H
#define CYLINDERLOOP_DEFINES_H

//void Error_Handler();

/**
 * @brief If execution failed more than __TIMES, jump to Error_Handler
 * @param
 */
#define ReliableHALExec(__ACTION, __TIMES) { \
    uint8_t execCnt = 1;                     \
    if((HAL_OK != __ACTION)&&++execCnt > __TIMES){ \
        Error_Handler();                     \
    }                                        \
}

/**
 * @brief If execution failed , return HAL_ERROR
 * @param
 */
#define TryHALConfig(__ACTION) {             \
    if(HAL_OK != __ACTION) return HAL_ERROR; \
}

#endif //CYLINDERLOOP_DEFINES_H
