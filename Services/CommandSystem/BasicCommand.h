//
// Created by bddwy on 2021/1/28.
//

#ifndef CYLINDERLOOP_BASICCOMMAND_H
#define CYLINDERLOOP_BASICCOMMAND_H

#include "includes.h"

class BasicCommand{
public:
    virtual void CommandDetect() = 0;
    void CommandExec(uint8_t commandID, uint8_t ParaNum, uint8_t* commandPara[]);
    void CommandExec(uint8_t* fullCommand);

private:
    uint8_t commandPartIndex;
};

#endif //CYLINDERLOOP_BASICCOMMAND_H
