//
// Created by bddwy on 2021/1/26.
//

#ifndef CYLINDERLOOP_DEFINES_H
#define CYLINDERLOOP_DEFINES_H

//void Error_Handler();

#define ReliableHALExec(__ACTION, __TIMES) { \
    uint8_t execCnt = 1;                     \
    if((HAL_OK != __ACTION)&&++execCnt > __TIMES){ \
        Error_Handler();                     \
    }                                        \
}

#endif //CYLINDERLOOP_DEFINES_H
