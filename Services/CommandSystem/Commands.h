//
// Created by bddwy on 2021/1/28.
//

#ifndef CYLINDERLOOP_COMMANDS_H
#define CYLINDERLOOP_COMMANDS_H

#include "includes.h"

template<uint8_t ArgSize>
struct PendingCommand{
    void (*action)(uint8_t argc, uint8_t* argv[]);
    uint8_t argc;
    uint8_t* argv[ArgSize];
    uint8_t prio;
};

template <uint8_t FIFOSize, uint8_t ArgSize>
struct CommandFIFO_t{
    PendingCommand<ArgSize> commands[FIFOSize];
    uint8_t head = 0, rear = 0;

    void CommandEnqueue(){

    }

    void CommandAutoExec(){

    }
};

template<uint8_t ArgSize>
struct Command_t{
    void (*action)(uint8_t argc, char* argv[]);
    PendingCommand<ArgSize>* commandFIFO;
    char name[];
};

#endif //CYLINDERLOOP_COMMANDS_H
