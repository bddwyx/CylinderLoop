//
// Created by bddwy on 2021/1/28.
//

#ifndef CYLINDERLOOP_ROBOTTASK_H
#define CYLINDERLOOP_ROBOTTASK_H

#ifdef __cplusplus
class Robot{
public:
    Robot() = default;

    void Reset();
    void Handle();
private:
};

extern Robot robot;

#endif //_cplusplus

#ifdef __cplusplus
extern "C" {
#endif //_cplusplus

    void MainInit();
    void MainControlLoop();

#ifdef __cplusplus
}
#endif //_cplusplus

#endif //CYLINDERLOOP_ROBOTTASK_H
