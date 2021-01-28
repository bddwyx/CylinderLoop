//
// Created by bddwy on 2021/1/28.
//

#include "RobotTask.h"
#include "Components.h"

Robot robot;

void Robot::Reset() {
    OLED_UI::oled_ui.Init();
    oled_LOGO();
}

void Robot::Handle() {
    //OLED_UI::oled_ui.Update();
}

extern "C"{
    void MainInit(){
        robot.Reset();
    }

    void MainControlLoop(){
        robot.Handle();
    }
}
