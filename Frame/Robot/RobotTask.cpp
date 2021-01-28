//
// Created by bddwy on 2021/1/28.
//

#include "RobotTask.h"
#include "Components.h"
#include "app_rtthread.h"
#include "rtconfig.h"

Robot robot;

void thread1Entry(){
    while(1){
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_2);
        rt_thread_delay(400);
    }
}

void thread2Entry(){
    while(1){
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_3);
        rt_thread_delay(300);
    }
}

static rt_thread_t led1Thread = RT_NULL;
static rt_thread_t led2Thread = RT_NULL;

void Robot::Reset() {
    led1Thread = rt_thread_create("led1",
                                  reinterpret_cast<void (*)(void *)>(thread1Entry),
                                  RT_NULL,
                                  512,
                                  3,
                                  20);
    rt_thread_startup(led1Thread);

    led2Thread = rt_thread_create("led2",
                                  reinterpret_cast<void (*)(void *)>(thread2Entry),
                                  RT_NULL,
                                  512,
                                  3,
                                  20);
    rt_thread_startup(led2Thread);

    OLED_UI::oled_ui.Init();
    OLED::_oled_device.Clear(Pen_Clear);
    OLED::_oled_device.DrawLine(0, 0, 100, 50, Pen_Write);
    //OLED::_oled_device.ShowLOGO();
    OLED::_oled_device.PrintString(2,0,"Hello");
    //OLED::_oled_device.DrawProgressBar(5, 50, 100, 10, 40);
    OLED::_oled_device.Clear(Pen_Inversion);
    OLED::_oled_device.OLEDRefreshBuffer();
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
