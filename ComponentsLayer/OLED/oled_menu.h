///**
//  ******************************************************************************
//  * @FileName               oled_menu.h
//  * @Description            OLED class with menu functions
//  * @author                 Liu Qi (purewhite@sjtu.edu.cn)
//  * @note                   Reconstructing...
//  ******************************************************************************
//  *
//  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
//  * All rights reserved.
//  *
//  ******************************************************************************
//**/
//#ifndef RM_FRAME2_OLED_MENU_H
//#define RM_FRAME2_OLED_MENU_H
//
//#include "includes.h"
//#include <stdint.h>
//#include <string.h>
//#include "oled_font.h"
//
///***	CONSTANT VALUE	***/
//#define ADC_CHANNALS (4)
//#define OLED_ADC_CHANNAL (0)
//
//#define X_WIDTH 128
//#define Y_WIDTH 64
//
//#define OLED_CMD 0x00
//#define OLED_DATA 0x01
//
//#define CHAR_SIZE_WIDTH_ASCII 6
//#define CHAR_SIZE_HIGHT_ASCII 12
//#define CHAR_SIZE_WIDTH_GB2312 12
//#define CHAR_SIZE_HIGHT_GB2312 12
//
//#define COL_BASE_PIXEL 4    // pixels available: [4, 123]
////#define ROW_BASE_PIXEL 2    // pixels available: [2, 61]
//#define MAX_COLUMN 20
//#define MAX_ROW 5
//#define CURSOR_BLINK_INTERVAL 20
//#define CURSOR_UPDATE_INTERVAL 10
//const uint8_t textLineStartY[] = {0, 13, 26, 39, 52};
//const uint8_t dividingLineY[] = {12, 25, 38, 51};
//
//const uint8_t PowOf2[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//
///***	TYPE DEFINE	***/
//
//typedef enum{
//    Pen_Clear = 0x00,   // Clear: Black, Write: White,  Inverse: black-white inverse.
//    Pen_Write = 0x01,
//    Pen_Inversion = 0x02,
//} Pen_e;
//
//class OLED{
//    friend class OLED_Menu;
//protected:
//    GPIO_TypeDef *DC_Port, *Rst_Port;
//    uint16_t DC_Pin, Rst_Pin;
//    uint8_t oledBuffer[128][64];
//
//    void CmdSet() {HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_SET);}
//    void CmdClr() {HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_RESET);}
//    void RstSet() {HAL_GPIO_WritePin(Rst_Port, Rst_Pin, GPIO_PIN_SET);}
//    void RstClr() {HAL_GPIO_WritePin(Rst_Port, Rst_Pin, GPIO_PIN_RESET);}
//    void OLEDWriteByte(uint8_t dat, uint8_t cmd);
//    void SetCursor(uint8_t x, uint8_t y);
//
//public:
//    static OLED _oled_device;
//
//    OLED() : DC_Port(OLED_DC_GPIO_Port), Rst_Port(OLED_RST_GPIO_Port), DC_Pin(OLED_DC_Pin), Rst_Pin(OLED_RST_Pin) {
//        for(uint8_t i = 0; i < 128; ++i)
//            memset(oledBuffer[i], 0, 64*sizeof(uint8_t));
//    }
//    OLED(GPIO_TypeDef *_DC_Port, uint16_t _DC_Pin, GPIO_TypeDef *_Rst_Port, uint16_t _Rst_Pin) : DC_Port(_DC_Port), Rst_Port(_Rst_Port), DC_Pin(_DC_Pin), Rst_Pin(_Rst_Pin) {
//        for(uint8_t i = 0; i < 128; ++i)
//            memset(oledBuffer[i], 0, 64*sizeof(uint8_t));
//    }
//    ~OLED();
//    void Init();
//    void DisplayOn();
//    void DisplayOff();
//    void OLEDBurstRefresh();
//
//    void Clear(Pen_e pen);
//    void Clear(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);
//    void Invert(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);
//    void DrawPoint(uint8_t x, uint8_t y, Pen_e pen);
//    void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_e pen);
//
//    void PrintASCII_Char(uint8_t row, uint8_t col, char chr);
//    void PrintGB2312_Char(uint8_t row, uint8_t col, uint8_t chr1, uint8_t chr2);
//    void PrintGB2312_Char(uint8_t row, uint8_t col, const char* chr);
//    void PrintNum(uint8_t row, uint8_t col, int32_t num, uint8_t mode, uint8_t len);
//    void PrintString(uint8_t row, uint8_t col, const char *chr);
//    void printf(uint8_t row, uint8_t col, const char *fmt,...);
//    void ShowLOGO();
//};
//
//
//class MenuTermBase{
//protected:
//    void Print() {}
//    void Pressed() {}
//    void Left() {}
//    void Right() {}
//};
//
//class OLED_UI{
//private:
//    static OLED device;
//    static OLED_UI oled_ui;
//    MenuTermBase **menu;
//    uint32_t currentStartLine;
//public:
//    OLED_UI() {
//        ;
//    }
//    OLED_UI(uint32_t num, ...) : menu(new MenuTermBase*[num]{nullptr}), currentStartLine(0) {
//        va_list args;
//        va_start(args, num);
//        for(uint32_t i = 0; i < num; ++i)
//            menu[i] = va_arg(args, MenuTermBase*);
//
//    }
//    ~OLED_UI();
//
//};
//
///***	SPECIFIC INIT CONFIGURATION	***/
//
///***	EXTERNAL VARIABLES	***/
//
///***	APIs	***/
//
//#endif //RM_FRAME2_OLED_MENU_H
