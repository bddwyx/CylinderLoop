/**
  ******************************************************************************
  * @FileName               oled.cpp
  * @Description            Implementation of oled.h
  * @author                 Xi Wang
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include "oled.h"
#include "app_rtthread.h"
#include "math.h"
#include <stdio.h>
#include <stdarg.h>

OLED OLED::_oled_device(&hspi1, OLED_DC_GPIO_Port, OLED_DC_Pin, OLED_RST_GPIO_Port, OLED_RST_Pin);


static rt_thread_t OLEDThread = RT_NULL;

void OLEDThreadEntry(void* para){
    while(1){
        OLED::_oled_device.OLEDRefresh();
        rt_thread_delay(5);
    }
}

void OLED::RTThreadInit() {
    auto entry = &OLEDThreadEntry;
    OLEDThread = rt_thread_create("OLED",
                                  entry,
                                  RT_NULL,
                                  512,
                                  3,
                                  2);
    rt_thread_startup(OLEDThread);
}

OLED::OLED(SPI_HandleTypeDef *hspi, GPIO_TypeDef *_DC_Port, uint16_t _DC_Pin, GPIO_TypeDef *_Rst_Port,
           uint16_t _Rst_Pin) : hspi(hspi), DC_Port(_DC_Port), Rst_Port(_Rst_Port), DC_Pin(_DC_Pin), Rst_Pin(_Rst_Pin) {
    for(int i = 0; i < 8; ++i) memset(oledBuffer[i], 0, 128 * sizeof(uint8_t));
}

 OLED::~OLED() {
    Clear(Pen_Clear);
    OLEDRefreshBuffer();
	DisplayOff();
}

/**
 * @brief Error handler for OLED
 * @param code: error code
 * @param msg: error message
 * @return (None)
 */
void OLED::Error(const char* msg, uint8_t code){
    //  TODO: I don`t know what to do when Error() is called. Thus a blank was left.
    //  If you have some ideas on it, please tell me.
		;
}

/**
 * @brief Write data/command to OLED
 * @param dat: the data/command ready to write
 * @param cmd: OLED_CMD(0x00) for command and OLDE_DATA(0x01) for data
 * @return (None)
 */
void OLED::OLEDWriteByte(uint8_t dat, uint8_t cmd) {
    static uint8_t dataBuffer;
    if(cmd != OLED_CMD)
        CmdSet();
    else
        CmdClr();
    dataBuffer = dat;
    HAL_SPI_Transmit_DMA(hspi, &dataBuffer, 1);
}

/**
 * @brief Set OLED cursor position
 * @param x: the X coordinate of cursor
 * @param y: the Y coordinate of cursor
 * @return (None)
 */
void OLED::SetCursor(uint8_t x, uint8_t y) {
    x += 2;
    OLEDWriteByte((0xb0 + y), OLED_CMD);
    OLEDWriteByte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLEDWriteByte((x & 0xf0), OLED_CMD);
}

/**
 * @brief Initialize the oled module
 * @param (None)
 * @return (None)
 */
void OLED::Init() {
    RstClr();
    HAL_Delay(500);
    RstSet();

    OLEDWriteByte(0xae, OLED_CMD);    //turn off oled panel
    OLEDWriteByte(0x00, OLED_CMD);    //set low column address
    OLEDWriteByte(0x10, OLED_CMD);    //set high column address
    OLEDWriteByte(0x40, OLED_CMD);    //set start line address
    OLEDWriteByte(0x81, OLED_CMD);    //set contrast control resigter
    OLEDWriteByte(0xcf, OLED_CMD);    //set SEG output current brightness
    OLEDWriteByte(0xa1, OLED_CMD);    //set SEG/column mapping
    OLEDWriteByte(0xc8, OLED_CMD);    //set COM/row scan direction
    OLEDWriteByte(0xa6, OLED_CMD);    //set nomarl display
    OLEDWriteByte(0xa8, OLED_CMD);    //set multiplex display
    OLEDWriteByte(0x3f, OLED_CMD);    //1/64 duty
    OLEDWriteByte(0xd3, OLED_CMD);    //set display offset
    OLEDWriteByte(0x00, OLED_CMD);    //not offest
    OLEDWriteByte(0xd5, OLED_CMD);    //set display clock divide ratio/oscillator frequency
    OLEDWriteByte(0x80, OLED_CMD);    //set divide ratio
    OLEDWriteByte(0xd9, OLED_CMD);    //set pre-charge period
    OLEDWriteByte(0xf1, OLED_CMD);    //pre-charge: 15 clocks, discharge: 1 clock
    OLEDWriteByte(0xda, OLED_CMD);    //set com pins hardware configuration
    OLEDWriteByte(0x12, OLED_CMD);    //
    OLEDWriteByte(0xdb, OLED_CMD);    //set vcomh
    OLEDWriteByte(0x40, OLED_CMD);    //set vcom deselect level
    OLEDWriteByte(0x20, OLED_CMD);    //set page addressing mode
    OLEDWriteByte(0x02, OLED_CMD);    //
    OLEDWriteByte(0x8d, OLED_CMD);    //set charge pump enable/disable
    OLEDWriteByte(0x14, OLED_CMD);    //charge pump disable
    OLEDWriteByte(0xa4, OLED_CMD);    //disable entire dispaly on
    OLEDWriteByte(0xa6, OLED_CMD);    //disable inverse display on
    OLEDWriteByte(0xaf, OLED_CMD);    //turn on oled panel

    OLEDWriteByte(0xaf, OLED_CMD);    //display on

    Clear(Pen_Clear);
    SetCursor(0, 0);

    RTThreadInit();
}

/**
 * @brief Turn on OLED display
 * @param (None)
 * @return (None)
 */
void OLED::DisplayOn() {
    OLEDWriteByte(0x8d, OLED_CMD);//set charge pump enable/disable
    OLEDWriteByte(0x14, OLED_CMD);//charge pump disable
    OLEDWriteByte(0xaf, OLED_CMD);//turn on oled panel
}

/**
 * @brief Turn off OLED display
 * @param (None)
 * @return (None)
 */
void OLED::DisplayOff() {
    OLEDWriteByte(0x8d, OLED_CMD);
    OLEDWriteByte(0x10, OLED_CMD);
    OLEDWriteByte(0xae, OLED_CMD);
}

/**
 * @brief Refresh the RAM of OLED
 * @param (None)
 * @return (None)
 */
void OLED::OLEDRefreshBuffer() {
	
    for (uint8_t i = 0; i < 8; ++i){
        SetCursor(0, i);
        CmdSet();
        HAL_SPI_Transmit(hspi, &(oledBuffer[i][0]), 2, 1000);
        HAL_SPI_Transmit_DMA(hspi, oledBuffer[i], 128);
        HAL_Delay(10);
    }
}

/**
 * @brief Clear the screen
 * @param (None)
 * @return (None)
 */
void OLED::Clear(Pen_e pen) {

    for(uint8_t i = 0; i < 8; ++i){
        for(uint8_t j = 0; j < 128; ++j){
            switch(pen){
                case Pen_Write:
                    oledBuffer[i][j] = 0xff;
                    break;
                case Pen_Clear:
                    oledBuffer[i][j] = 0x00;
                    break;
                case Pen_Inversion:
                    oledBuffer[i][j] = 0xff - oledBuffer[i][j];
                    break;
                default:
                    Error("Clear(): Invalid pen type.\n", 2);
										return;
            }
        }
    }
}

/**
 * @brief Clear a given area
 * @param x: the X coordinate of upper-left point of the area
 * @param y: the Y coordinate of upper-left point of the area
 * @param width: the width the area
 * @param height: the height of the area
 * @return (None)
 */
void OLED::Clear(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
    //uint8_t page, row;
    for(uint8_t x = x0; x < x0+width; ++x)
        for(uint8_t y = y0; y < y0+height; ++y){
					/*
            page = y / 8;
						row = y % 8;
            oledBuffer[x][page] ^= PowOf2[row];
					*/
					DrawPoint(x, y, Pen_Clear);
        }
}

/**
 * @brief Invert the color of pixels in a given area
 * @param x: the X coordinate of upper-left point of the area
 * @param y: the Y coordinate of upper-left point of the area
 * @param width: the width the area
 * @param height: the height of the area
 * @return (None)
 */
void OLED::Invert(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
    //uint8_t page, row;
    for(uint8_t x = x0; x < x0+width; ++x)
        for(uint8_t y = y0; y < y0+height; ++y){
					/*
            page = y / 8;
						row = y % 8;
            oledBuffer[x][page] ^= PowOf2[row];
					*/
					DrawPoint(x, y, Pen_Inversion);
        }
}

/**
 * @brief Draw a point of specific pen at (x, y)
 * @param x: the X coordinate of cursor
 * @param y: the Y coordinate of cursor
 * @param pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e, the type of the pen
 * @return (None)
 */
void OLED::DrawPoint(uint8_t x, uint8_t y, Pen_e pen) {
    uint8_t page = 0, row = 0;

    /* check the corrdinate */
    if((x > (X_WIDTH - 1)) || (y > (Y_WIDTH - 1))){
        Error("DrawPoint(): Invalid coordinate.\n", 1);
				return;
		}
    page = y / 8;
    row = y % 8;

    switch(pen){
        case Pen_Clear:
            oledBuffer[page][x] &= ~(1 << row);
            break;
        case Pen_Write:
            oledBuffer[page][x] |= 1 << row;
            break;
        case Pen_Inversion:
            oledBuffer[page][x] ^= 1 << row;
            break;
        default:
            Error("DrawPoint(): Invalid pen type.\n", 2);
						return;
    }
}

/**
 * @brief Draw a line from (x1, y1) to (x2, y2)
 * @param x1, y1: the start point of line
 * @param x2, y2: the end of line
 * @param pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e, the type of the pen
 * @return (None)
 */
void OLED::DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_e pen) {
    uint8_t col = 0, row = 0;
    uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
    float k = 0.0f, b = 0.0f;

    if(y1 == y2){
        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x1);

        for (col = x_st; col <= x_ed; col++)
            DrawPoint(col, y1, pen);
    }
    else if(x1 == x2){
        (y1 <= y2) ? (y_st = y1):(y_st = y2);
        (y1 <= y2) ? (y_ed = y2):(y_ed = y1);

        for (row = y_st; row <= y_ed; row++)
            DrawPoint(x1, row, pen);
    }
    else{
        k = ((float)(y2 - y1)) / (x2 - x1);
        b = (float)y1 - k * x1;

        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x2);

        for (col = x_st; col <= x_ed; ++col)
            DrawPoint(col, (uint8_t)(col * k + b), pen);
    }
}

/**
 * @brief Draw the border of a rectangle at the given location
 * @param x,y: coordinate of the upper-left vertex of the rectangle
 * @param width: width of the rectangle
 * @param height: height of the rectangle
 * @param pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e, the type of the pen
 * @return (None)
 */
void OLED::DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, Pen_e pen){
    DrawLine(x, y, x+width, y, pen);
    DrawLine(x, y, x, y+height, pen);
    DrawLine(x+width, y, x+width, y+height, pen);
    DrawLine(x, y+height, x+width, y+height, pen);
}

/**
 * @brief Draw a filled rectangle at the given location
 * @param x,y: coordinate of the upper-left vertex of the rectangle
 * @param width: width of the rectangle
 * @param height: height of the rectangle
 * @param pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e, the type of the pen
 * @return (None)
 */
void OLED::FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, Pen_e pen){
    for(int i = 0; i < height; ++i)
        DrawLine(x, y+i, x+width, y+i, pen);
}

/**
 * @brief Draw the border of a circle
 * @param x0,y0: coordinate of the center of the rectangle
 * @param radius: radius of the circle
 * @param pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e, the type of the pen
 * @return (None)
 */
void OLED::DrawCircle(uint8_t x0, uint8_t y0, uint8_t radius, Pen_e pen){
    int8_t x = 0, y = radius;
	int8_t dp = 1 - radius;
	do {
		if (dp < 0)
			dp = dp + (x++) * 2 + 3;    // TODO: What does it mean?
		else
			dp = dp + (x++) * 2 - (y--) * 2 + 5;

		DrawPoint(x0 + x, y0 + y, pen);     //For the 8 octants
		DrawPoint(x0 - x, y0 + y, pen);
		DrawPoint(x0 + x, y0 - y, pen);
		DrawPoint(x0 - x, y0 - y, pen);
		DrawPoint(x0 + y, y0 + x, pen);
		DrawPoint(x0 - y, y0 + x, pen);
		DrawPoint(x0 + y, y0 - x, pen);
		DrawPoint(x0 - y, y0 - x, pen);
	} while (x < y);

    DrawPoint(x0 + radius, y0, pen);
    DrawPoint(x0, y0 + radius, pen);
    DrawPoint(x0 - radius, y0, pen);
    DrawPoint(x0, y0 - radius, pen);
}

/**
 * @brief Draw a filled circle
 * @param x0,y0: coordinate of the center of the rectangle
 * @param radius: radius of the circle
 * @param pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e, the type of the pen
 * @return (None)
 */
void OLED::FillCircle(uint8_t x0, uint8_t y0, uint8_t radius, Pen_e pen){
    int16_t x = 0, y = radius;
	int16_t dp = 1 - radius;
	do{
		if (dp < 0)
            dp = dp + (x++) * 2 + 3;    // TODO: What does it mean?
        else
            dp = dp + (x++) * 2 - (y--) * 2 + 5;
        
        DrawLine(x0 - x, y0 - y, x0 + x, y0 - y, pen);
        DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, pen);
        DrawLine(x0 - y, y0 - x, x0 + y, y0 - x, pen);
        DrawLine(x0 - y, y0 + x, x0 + y, y0 + x, pen);
	} while (x < y);
    DrawLine(x0 - radius, y0, x0 + radius, y0, pen);
}

/**
 * @brief Draws a rectangular progress bar with the outer dimensions given by width and height
 * @param x0,y0: coordinate of the upper-left vertex of the rectangular bar
 * @param width: width of the rectangular bar
 * @param height: height of the rectangular bar
 * @param progress: an unsigned byte value between 0 and 100
 * @return (None)
 */
void OLED::DrawProgressBar(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t progress){
    DrawRectangle(x0, y0, width, height, Pen_Write);
    FillRectangle(x0+1, y0+1, width-2, height-2, Pen_Clear);
    FillRectangle(x0, y0, width*progress/100, height, Pen_Write);
}

/**
 * @brief Draw a rectangular bitmap in the internal image format
 * @param x0,y0: coordinate of the upper-left vertex of the rectangular image
 * @param width: width of the rectangular image
 * @param height: height of the rectangular image
 * @param image: bitmap image to draw
 * @return (None)
 */
void OLED::DrawImage(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, const uint8_t *image){
    uint8_t mask = 128, buffer = image[0];
    uint32_t position = 0;
    for(uint8_t y = y0; y < y0+height; ++y){
        for(uint8_t x = x0; x < x0+width; ++x){
            if(buffer & mask) DrawPoint(x, y, Pen_Write);
            else DrawPoint(x, y, Pen_Clear);
            mask >>= 1;
            if(mask == 0){
                mask = 128;
                buffer = image[++position];
            }
        }
    }
}

/**
 * @brief Print a ascii character
 * @param row: row of the character
 * @param col: column of the character
 * @param chr: the character ready to print
 * @note �������д����Щ��֣����룺ʹ�õ�ascii��ģ���뷽ʽ�Ƚ���֣����?�Ǵ�y�������ģ����Y_LEN=12��1 byte = 8 bit��������Ҫ����д��
 * @return (None)
 */
void OLED::PrintASCII_Char(uint8_t row, uint8_t col, char chr) {
    uint8_t x = col * 6;
    uint8_t y = row * 12;
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr -= ' ';

    for (t = 0; t < 12; ++t){
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; ++t1){
            if(temp&0x80)
                DrawPoint(x, y, Pen_Write);
            else
                DrawPoint(x, y, Pen_Clear);

            temp <<= 1;
            ++y;
            if((y - y0) == 12){
                y = y0;
                ++x;
                break;
            }
        }
    }
}

/**
 * @brief Print a GB2312 character
 * @param row: row of the character
 * @param col: column of the character
 * @param chr1, chr2: the character ready to print. chr1: 
 * @return (None)
 */
void OLED::PrintGB2312_Char(uint8_t row, uint8_t col, uint8_t chr1, uint8_t chr2){
    uint8_t x = col * 6, y = row * 12, buffer[24];
    uint32_t offset = (94u*((uint8_t)chr1-0xa0-1) + ((uint8_t)chr2-0xa0-1))*24;
    memcpy(buffer, gb2312_1212+offset, 24*sizeof(uint8_t));
    for(int i = 0; i < 12; ++i){
        for(int j = 0; j < 8; ++j){
            if(buffer[i*2] & PowOf2[j])
                DrawPoint(x, y, Pen_Write);
            else
                DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        for(int j = 0; j < 4; ++j){
            if(buffer[i*2+1] & PowOf2[j])
                DrawPoint(x, y, Pen_Write);
            else
                DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        ++y;
        x -= 12;
    }
}

/**
 * @brief Print a GB2312 character
 * @param row: row of the character
 * @param col: column of the character
 * @param chr: the character to output
 * @return (None)
 */
void OLED::PrintGB2312_Char(uint8_t row, uint8_t col, const char* chr){
    uint8_t x = col * 6, y = row * 12, buffer[24];
    uint32_t offset =(94u*((uint8_t)chr[0]-0xa0-1) + ((uint8_t)chr[1]-0xa0-1))*24;
    memcpy(buffer, gb2312_1212+offset, 24*sizeof(uint8_t));
    for(int i = 0; i < 12; ++i){
        for(int j = 0; j < 8; ++j){
            if(buffer[i*2] & PowOf2[j])
                DrawPoint(x, y, Pen_Write);
            else
                DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        for(int j = 0; j < 4; ++j){
            if(buffer[i*2+1] & PowOf2[j])
                DrawPoint(x, y, Pen_Write);
            else
                DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        ++y;
        x -= 12;
    }
}

/**
 * @brief Print a number
 * @param row: row of the number
 * @param col: column of the number
 * @param num: the number ready to print
 * @param mode: 0x00, fill number with spaces; otherwise(esp. 0x01), fill number with '0'.
 * @param len: the length of the number
 * @return (None)
 */
void OLED::PrintNum(uint8_t row, uint8_t col, int32_t num, uint8_t mode, uint8_t len) {
    uint8_t numberString[16], maxDigit = 0, i, j;
    bool isNegative = (num < 0);
    while(num){
        numberString[maxDigit++] = num % 10;
        num /= 10;
    }

    if(maxDigit + ((isNegative) ? 1 : 0) > len){
				Error("PrintNum(): len is too short.\n", 3);
				return;
		}

    if(isNegative){
        PrintASCII_Char(row, col, '-');
        ++col;
        --len;
    }

    for(i = 0; i + maxDigit < len; ++i)
        PrintASCII_Char(row, col+i, ((mode == 0x00) ? ' ' : '0'));
    for(j = 0; j < maxDigit; ++j)
        PrintASCII_Char(row, col+i+j, numberString[maxDigit-j-1]+'0');
}

/**
 * @brief Print a character string
 * @param row: row where the character string begins
 * @param col: column where the character string begins
 * @param str: the pointer to character string
 * @note please use GB2312 encoding when you`re going to output Chinese characters.
 * @return (None)
 */
void OLED::PrintString(uint8_t row, uint8_t col, const char *str) {
    for(uint8_t i = 0; str[i] != '\0'; ++i){
        if((signed char)str[i] > 0){     // ASCII
            if(col >= 20){
								Error("PrintString(): Invalid coordinate for ASCII Characters.\n", 4);
								return;
						}
						if(str[i] == '\n'){
								++row;
								continue;
						}
            PrintASCII_Char(row, col, str[i]);
            ++col;
            if(col > 20){
                col = 0;
                ++row;
            }
        }
        else{               // GB2312
            if(col >= 19){
								Error("PrintString(): Invalid coordinate for GB2312 Characters.\n", 4);
								return;
						}
            PrintGB2312_Char(row, col, str[i], str[i+1]);
            ++i;
            col += 2;
            if(col > 20){
                col = 0;
                ++row;
            }
        }
    }
}

/**
 * @brief Formatted output in oled 128*64
 * @param row: row of character string begin, 0 <= row <= 4;
 * @param col: column of character string begin, 0 <= col <= 20;
 * @param *fmt: the pointer to format character string
 * @return (None)
 * @note if the character length is more than one row at a time, the extra characters will be truncated
 * @note Chinese characters are 2-byte wide thus %c is not suitable
 * @note please use GB2312 encoding when you`re going to output Chinese characters.
 */
void OLED::printf(uint8_t row, uint8_t col, const char *fmt, ...) {
    char OLED_Buffer[128] = {0};
    uint8_t remainingSize = 0;
    va_list ap;

    if((row > 4) || (row < 1) || (col > 20) || (col < 1)){
        Error("printf(): Invalid coordinate.\n", 1);
				return;
		}

    va_start(ap, fmt);

    vsprintf((char *)OLED_Buffer, fmt, ap);

    va_end(ap);

    remainingSize = 21 - col;

    OLED_Buffer[remainingSize] = '\0';

    PrintString(row, col, OLED_Buffer);
}

/**
 * @brief Show the logo of dji Robomaster   // TODO: Rewrite the logo array with JiaoLoong`s
 * @param (None)
 * @return (None)
 * @note the LOGO_BMP is removed from oled.h, thus this function is abandoned.
 */

void OLED::ShowLOGO() {
    Clear(Pen_Clear);
    uint8_t temp_char = 0;
    uint8_t x = 0, y = 0;
    uint8_t i = 0;
    for(; y < 64; y += 8){
        for(x = 0; x < 128; ++x){
            temp_char = LOGO_BMP[x][y/8];
            for(i = 0; i < 8; ++i){
                if(temp_char & 0x80) DrawPoint(x, y + i,Pen_Write);
                else DrawPoint(x,y + i,Pen_Clear);
                temp_char <<= 1;
            }
        }
    }
}

void OLED::OLEDRefresh() {
    static uint8_t row = 0;

    SetCursor(0, row);
    CmdSet();
    HAL_SPI_Transmit(hspi, &(oledBuffer[row][0]), 2, 1000);
    HAL_SPI_Transmit_DMA(hspi, oledBuffer[row], 128);

    row++;
    row %= Y_WIDTH / 8;
}
