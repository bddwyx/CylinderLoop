//
// Created by bddwy on 2021/1/28.
//

#include "RobotTask.h"
#include "Components.h"
#include "app_rtthread.h"

Robot robot;


int Utf8ToUnicode(char* pInput, char* pOutput) {
    int outputSize = 0; //记录转换后的Unicode字符串的字节数

    while (*pInput) {
        if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
        {
            *pOutput = *pInput;
            pOutput++;
            *pOutput = 0; //小端法表示，在高地址填补0
        } else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char low = *pInput;
            if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
            {
                return -1; //如果不是则报错
            }

            *pOutput = (high << 6) + (low & 0x3F);
            pOutput++;
            *pOutput = (high >> 2) & 0x07;
        } else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
        {
            char high = *pInput;
            pInput++;
            char middle = *pInput;
            pInput++;
            char low = *pInput;
            if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80)) {
                return -1;
            }
            *pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位
            pOutput++;
            *pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
        } else //对于其他字节数的UTF8字符不进行处理
        {
            return -1;
        }
        pInput++;//处理下一个utf8字符
        pOutput++;
        outputSize += 2;
    }
    //unicode字符串后面，有两个\0
    *pOutput = 0;
    pOutput++;
    *pOutput = 0;
    return outputSize;
}

void thread1Entry(void* para){
    while(1){
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_2);
        rt_thread_delay(400);
    }
}

void thread2Entry(void* para){
    while(1){
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_3);
        rt_thread_delay(300);
    }
}

void BarThreadEntry(void* para){
    static uint8_t progress = 0;
    while(1){
        OLED::_oled_device.DrawProgressBar(13, 50, 100, 10, progress);
        progress = ++progress % 100;
        rt_thread_delay(40);
    }
}

static rt_thread_t led1Thread = RT_NULL;
static rt_thread_t led2Thread = RT_NULL;
static rt_thread_t BarThread = RT_NULL;

void Robot::Reset() {
    led1Thread = rt_thread_create("led1",
                                  thread1Entry,
                                  RT_NULL,
                                  512,
                                  3,
                                  20);
    rt_thread_startup(led1Thread);

    led2Thread = rt_thread_create("led2",
                                  thread2Entry,
                                  RT_NULL,
                                  512,
                                  3,
                                  20);
    rt_thread_startup(led2Thread);

    BarThread = rt_thread_create("Bar",
                                  BarThreadEntry,
                                  RT_NULL,
                                  512,
                                  3,
                                  20);
    rt_thread_startup(BarThread);

    OLED::_oled_device.Init();
    //OLED::_oled_device.Clear(Pen_Clear);
    OLED::_oled_device.ShowLOGO();
    OLED::_oled_device.DrawLine(0, 0, 100, 50, Pen_Write);
    const char* a = new char [4]{0xD2, 0xBB, 0, 0};
    OLED::_oled_device.PrintString(2, 0, a);
    //LED::_oled_device.printf(2, 0, "hello");
    //OLED::_oled_device.Clear(Pen_Inversion);
    //OLED::_oled_device.OLEDBurstRefresh();
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
