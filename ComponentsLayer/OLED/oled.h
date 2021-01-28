/**
  ******************************************************************************
  * @FileName               oled.h
  * @Description            OLED class with basic operating functions
  * @author                 Liu Qi (purewhite@sjtu.edu.cn)
  * @note                   UNSTABLE VERSION!
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
#define USE_DJI_EXAMPLE_CODE

#include "stm32f4xx.h"
#include "spi.h"
#include <stdint.h>
#include <string.h>
#include "oled_font.h"

/***	MAP	***/

/***	CONSTANT VALUE	***/
#define X_WIDTH 128
#define Y_WIDTH 64

#define OLED_CMD 0x00
#define OLED_DATA 0x01

#define CHAR_SIZE_WIDTH_ASCII 6
#define CHAR_SIZE_HIGHT_ASCII 12
#define CHAR_SIZE_WIDTH_GB2312 12
#define CHAR_SIZE_HIGHT_GB2312 12

const uint8_t PowOf2[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

#ifdef USE_DJI_EXAMPLE_CODE
static uint8_t oledBuffer[128][8];
#endif
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
        GPIO_TypeDef *DC_Port, *Rst_Port;
        uint16_t DC_Pin, Rst_Pin;
				#ifndef USE_DJI_EXAMPLE_CODE
        uint8_t oledBuffer[128][8];
				#endif
        void Error(const char* msg, uint8_t code);

        void CmdSet() {HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_SET);}
        void CmdClr() {HAL_GPIO_WritePin(DC_Port, DC_Pin, GPIO_PIN_RESET);}
        void RstSet() {HAL_GPIO_WritePin(Rst_Port, Rst_Pin, GPIO_PIN_SET);}
        void RstClr() {HAL_GPIO_WritePin(Rst_Port, Rst_Pin, GPIO_PIN_RESET);}
        void OLEDWriteByte(uint8_t dat, uint8_t cmd);
        void SetCursor(uint8_t x, uint8_t y);

    public:
        static OLED _oled_device;

        OLED() : DC_Port(OLED_DC_GPIO_Port), Rst_Port(OLED_RST_GPIO_Port), DC_Pin(OLED_DC_Pin), Rst_Pin(OLED_RST_Pin) {}
        OLED(GPIO_TypeDef *_DC_Port, uint16_t _DC_Pin, GPIO_TypeDef *_Rst_Port, uint16_t _Rst_Pin) : DC_Port(_DC_Port), Rst_Port(_Rst_Port), DC_Pin(_DC_Pin), Rst_Pin(_Rst_Pin) {
            for(int i = 0; i < 128; ++i)
                memset(oledBuffer[i], 0, 8 * sizeof(uint8_t));
        }
        ~OLED();
        void Init();
        void DisplayOn();
        void DisplayOff();
        void OLEDRefreshBuffer();

        void Clear(Pen_e pen);
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
        void ShowLOGO();
};

/***	SPECIFIC INIT CONFIGURATION	***/

/***	EXTERNAL VARIABLES	***/

/***	APIs	***/
// These functions are copied from dji official example.
#ifdef USE_DJI_EXAMPLE_CODE
void oled_init(void);
void oled_write_byte(uint8_t dat, uint8_t cmd);
void oled_display_on(void);
void oled_display_off(void);
void oled_refresh_gram(void);
void oled_clear(Pen_e pen);
void oled_drawpoint(uint8_t x, uint8_t y, Pen_e pen);
void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_e pen);
void oled_showchar(uint8_t row, uint8_t col, uint8_t chr);
void oled_shownum(uint8_t row, uint8_t col, uint32_t num, uint8_t mode, uint8_t len);
void oled_showstring(uint8_t row, uint8_t col, uint8_t *chr);
void oled_printf(uint8_t row, uint8_t col, const char *fmt,...);
void oled_LOGO(void);
#endif

#endif
