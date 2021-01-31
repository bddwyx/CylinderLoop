/**
  ******************************************************************************
  * @FileName               oled.h
  * @Description            OLED class with basic operating functions
  * @author                 Xi Wang
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

/**
 * OLED flash Address:
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
**/

#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "spi.h"
#include <stdint.h>
#include <string.h>
#include "OLEDGraphicLib_font.h"
#include "OLEDGraphicLib_figure.h"

/***	MAP	***/

/***	CONSTANT VALUE	***/
constexpr uint8_t X_WIDTH = 128;
constexpr uint8_t Y_WIDTH = 64;

constexpr uint8_t OLED_CMD = 0x00;
constexpr uint8_t OLED_DATA = 0x01;

#define CHAR_SIZE_WIDTH_ASCII 6
#define CHAR_SIZE_HIGHT_ASCII 12
#define CHAR_SIZE_WIDTH_GB2312 12
#define CHAR_SIZE_HIGHT_GB2312 12

constexpr uint8_t PowOf2[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

/***	TYPE DEFINE	***/

typedef enum{
    Pen_Clear = 0x00,   // Guess: Clear: Black, Write: White,  Inverse: black-white inverse.
    Pen_Write = 0x01,
    Pen_Inversion = 0x02,
} Pen_e;

class OLED{
		friend class OLED_Menu;
		friend class OLED_UI;
    protected:
        SPI_HandleTypeDef* hspi;
        GPIO_TypeDef *DC_Port, *Rst_Port;
        uint16_t DC_Pin, Rst_Pin;
        uint8_t oledBuffer[8][128];
        void Error(const char* msg, uint8_t code);

        void CmdSet() {HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_SET);}
        void CmdClr() {HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_RESET);}
        void Reset();
        void OLEDWriteByte(uint8_t dat, uint8_t cmd);
        void SetCursor(uint8_t x, uint8_t y);

    public:
        static OLED _oled_device;

        OLED(SPI_HandleTypeDef *hspi, GPIO_TypeDef *_DC_Port, uint16_t _DC_Pin, GPIO_TypeDef *_Rst_Port,
             uint16_t _Rst_Pin);
        ~OLED();
        void Init();
        friend void OLEDThreadEntry(void* para);
        void RTThreadInit();
        void DisplayOn();
        void DisplayOff();
        void OLEDRefresh();
        void OLEDBurstRefresh();

        void FullScreenOperation(Pen_e pen);
        void Clear(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);
        void Invert(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);
        void DrawPoint(uint8_t x, uint8_t y, Pen_e pen);
        void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_e pen);
        void DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, Pen_e pen);
        void FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, Pen_e pen);
        void DrawCircle(uint8_t x, uint8_t y, uint8_t radius, Pen_e pen);
        void FillCircle(uint8_t x, uint8_t y, uint8_t radius, Pen_e pen);
        void DrawProgressBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t progress);
        void DrawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *image);

        void PrintASCII_Char(uint8_t row, uint8_t col, char chr);
        void PrintGB2312_Char(uint8_t row, uint8_t col, uint8_t chr1, uint8_t chr2);
				void PrintGB2312_Char(uint8_t row, uint8_t col, const char* chr);
        void PrintNum(uint8_t row, uint8_t col, int32_t num, uint8_t mode, uint8_t len);
        void PrintString(uint8_t row, uint8_t col, const char *chr);
        void printf(uint8_t row, uint8_t col, const char *fmt,...);

        void ShowTest();
};



#endif
