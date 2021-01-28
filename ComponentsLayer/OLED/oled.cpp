/**
  ******************************************************************************
  * @FileName               oled.cpp
  * @Description            Implementation of oled.h
  * @author                 Liu Qi (purewhite@sjtu.edu.cn)
  * @note                   UNSTABLE VERSION!
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include "oled.h"
#include "math.h"
#include <stdio.h>
#include <stdarg.h>

OLED OLED::_oled_device;

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
    HAL_SPI_Transmit_DMA(&hspi1, &dataBuffer, 1);
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
        for (uint8_t j = 0; j <= 128; ++j){
            OLEDWriteByte(oledBuffer[j][i], OLED_DATA);
        }
    }

//    CmdClr();
//    HAL_SPI_Transmit_DMA(&hspi1, *oledBuffer, 128);
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
                    oledBuffer[j][i] = 0xff;
                    break;
                case Pen_Clear:
                    oledBuffer[j][i] = 0x00;
                    break;
                case Pen_Inversion:
                    oledBuffer[j][i] = 0xff - oledBuffer[j][i];
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
            oledBuffer[x][page] &= ~(1 << row);
            break;
        case Pen_Write:
            oledBuffer[x][page] |= 1 << row;
            break;
        case Pen_Inversion:
            oledBuffer[x][page] ^= 1 << row;
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

// TODO: add rectangle, fillrectangle, circle, fillcircle (dji didn`t develop.)

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

#ifdef USE_DJI_EXAMPLE_CODE
#define OLED_CMD_Set()      HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define OLED_CMD_Clr()      HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)

#define OLED_RST_Set()      HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)
#define OLED_RST_Clr()      HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET)

/**
 * @brief   write data/command to OLED
 * @param   dat: the data ready to write
 * @param   cmd: 0x00,command 0x01,data
 * @retval  
 */
void oled_write_byte(uint8_t dat, uint8_t cmd)
{
    if (cmd != 0)
        OLED_CMD_Set();
    else
        OLED_CMD_Clr();

    HAL_SPI_Transmit(&hspi1, &dat, 1, 10);
}


/**
 * @brief   set OLED cursor position
 * @param   x: the X-axis of cursor
 * @param   y: the Y-axis of cursor
 * @retval  
 */
static void oled_set_pos(uint8_t x, uint8_t y)
{
    x += 2;
    oled_write_byte((0xb0 + y), OLED_CMD);              //set page address y
    oled_write_byte(((x&0xf0)>>4)|0x10, OLED_CMD);      //set column high address
    oled_write_byte((x&0xf0), OLED_CMD);                //set column low address
}

/**
 * @brief   turn on OLED display
 * @param   None
 * @param   None
 * @retval  
 */
void oled_display_on(void)
{
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x14, OLED_CMD);
    oled_write_byte(0xaf, OLED_CMD);
}

/**
 * @brief   turn off OLED display
 * @param   None
 * @param   None
 * @retval  
 */
void oled_display_off(void)
{
    oled_write_byte(0x8d, OLED_CMD);
    oled_write_byte(0x10, OLED_CMD);
    oled_write_byte(0xae, OLED_CMD);
}

/**
 * @brief   refresh the RAM of OLED
 * @param   None
 * @param   None
 * @retval  
 */
void oled_refresh_gram(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        oled_set_pos(0, i);

        for (n = 0; n < 128; n++)
        {
            oled_write_byte(oledBuffer[n][i], OLED_DATA);
        }
    }
}

/**
 * @brief   clear the screen
 * @param   None
 * @param   None
 * @retval  
 */
void oled_clear(Pen_e pen)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            if (pen == Pen_Write)
                oledBuffer[n][i] = 0xff;
            else if (pen == Pen_Clear)
                oledBuffer[n][i] = 0x00;
            else
                oledBuffer[n][i] = 0xff - oledBuffer[n][i];
        }
    }
}

/**
 * @brief   draw a point at (x, y)
 * @param   x: the X-axis of cursor
 * @param   y: the Y-axis of cursor
 * @param   pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e
 * @retval  None
 */
void oled_drawpoint(uint8_t x, uint8_t y, Pen_e pen)
{
    uint8_t page = 0, row = 0;

    /* check the corrdinate */
    if ((x > (X_WIDTH - 1)) || (y > (Y_WIDTH - 1)))
        return;

    page = y / 8;
    row = y % 8;

    if (pen == Pen_Write)
        oledBuffer[x][page] |= 1 << row;
    else if (pen == Pen_Inversion)
        oledBuffer[x][page] ^= 1 << row;
    else
        oledBuffer[x][page] &= ~(1 << row);
}

/**
 * @brief   draw a line from (x1, y1) to (x2, y2)
 * @param   x1, y1: the start point of line
 * @param   x2, y2: the end of line
 * @param   pen: Pen_Clear, Pen_Write, Pen_Inversion @Pen_e
 * @retval  None
 */
void oled_drawline(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Pen_e pen)
{
    uint8_t col = 0, row = 0;
    uint8_t x_st = 0, x_ed = 0, y_st = 0, y_ed = 0;
    float k = 0.0f, b = 0.0f;

    if (y1 == y2)
    {
        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x1);

        for (col = x_st; col <= x_ed; col++)
        {
            oled_drawpoint(col, y1, pen);
        }
    }
    else if (x1 == x2)
    {
        (y1 <= y2) ? (y_st = y1):(y_st = y2);
        (y1 <= y2) ? (y_ed = y2):(y_ed = y1);

        for (row = y_st; row <= y_ed; row++)
        {
            oled_drawpoint(x1, row, pen);
        }
    }
    else
    {
        k = ((float)(y2 - y1)) / (x2 - x1);
        b = (float)y1 - k * x1;

        (x1 <= x2) ? (x_st = x1):(x_st = x2);
        (x1 <= x2) ? (x_ed = x2):(x_ed = x2);

        for (col = x_st; col <= x_ed; col++)
        {
            oled_drawpoint(col, (uint8_t)(col * k + b), pen);
        }
    }
}


//To add: rectangle, fillrectangle, circle, fillcircle, 

/**
 * @brief   show a character
 * @param   row: row of character
 * @param   col: column of character
 * @param   chr: the character ready to show
 * @retval  None
 */
void oled_showchar(uint8_t row, uint8_t col, uint8_t chr)
{
    uint8_t x = col * 6;
    uint8_t y = row * 12;
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr = chr - ' ';

    for (t = 0; t < 12; t++)
    {
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp&0x80)
                oled_drawpoint(x, y, Pen_Write);
            else
                oled_drawpoint(x, y, Pen_Clear);

            temp <<= 1;
            y++;
            if ((y - y0) == 12)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

//m^n
static uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)
        result *= m;

    return result;
}

/**
 * @brief   show a number
 * @param   row: row of number
 * @param   col: column of number
 * @param   num: the number ready to show
 * @param   mode: 0x01, fill number with '0'; 0x00, fill number with spaces
 * @param   len: the length of the number
 * @retval  None
 */
void oled_shownum(uint8_t row, uint8_t col, uint32_t num, uint8_t mode, uint8_t len)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t -1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode == 0)
                    oled_showchar(row, col + t, ' ');
                else
                    oled_showchar(row, col + t, '0');
                continue;
            }
            else
                enshow = 1;
        }

        oled_showchar(row, col + t, temp + '0');
    }
}


/**
 * @brief   show a character string
 * @param   row: row of character string begin
 * @param   col: column of character string begin
 * @param   chr: the pointer to character string
 * @retval  None
 */
void oled_showstring(uint8_t row, uint8_t col, uint8_t *chr)
{
    uint8_t n =0;

    while (chr[n] != '\0')
    {
        oled_showchar(row, col, chr[n]);
        col++;

        if (col > 20)
        {
            col = 0;
            row += 1;
        }
        n++;
    }
}

/**
 * @brief   formatted output in oled 128*64
 * @param   row: row of character string begin, 0 <= row <= 4;
 * @param   col: column of character string begin, 0 <= col <= 20;
 * @param   *fmt: the pointer to format character string
 * @retval  None
 * @note    if the character length is more than one row at a time, the extra characters will be truncated
 */
void oled_printf(uint8_t row, uint8_t col, const char *fmt,...)
{
    uint8_t LCD_BUF[128] = {0};
    uint8_t remain_size = 0;
    va_list ap;

    if ((row > 4) || (row < 1) || (col > 20) || (col < 1))
        return;

    va_start(ap, fmt);

    vsprintf((char *)LCD_BUF, fmt, ap);

    va_end(ap);

    remain_size = 21 - col;

    LCD_BUF[remain_size] = '\0';

    oled_showstring(row, col, LCD_BUF);
}

void oled_LOGO(void)
{
    oled_clear(Pen_Clear);
    uint8_t temp_char = 0;
    uint8_t x = 0, y = 0;
    uint8_t i = 0;
    for(; y < 64; y += 8)
    {
        for(x = 0; x < 128; x++)
        {
            temp_char = LOGO_BMP[x][y/8];
            for(i = 0; i < 8; i++)
            {
                if(temp_char & 0x80) oled_drawpoint(x, y + i,Pen_Write);
                else oled_drawpoint(x,y + i,Pen_Clear);
                temp_char <<= 1;
            }
        }
    }
    oled_refresh_gram();
}

/**
 * @brief   initialize the oled module
 * @param   None
 * @retval  None
 */
void oled_init(void)
{
    OLED_RST_Clr();
    HAL_Delay(500);
    OLED_RST_Set();

    oled_write_byte(0xae, OLED_CMD);    //turn off oled panel
    oled_write_byte(0x00, OLED_CMD);    //set low column address
    oled_write_byte(0x10, OLED_CMD);    //set high column address
    oled_write_byte(0x40, OLED_CMD);    //set start line address
    oled_write_byte(0x81, OLED_CMD);    //set contrast control resigter
    oled_write_byte(0xcf, OLED_CMD);    //set SEG output current brightness
    oled_write_byte(0xa1, OLED_CMD);    //set SEG/column mapping
    oled_write_byte(0xc8, OLED_CMD);    //set COM/row scan direction
    oled_write_byte(0xa6, OLED_CMD);    //set nomarl display
    oled_write_byte(0xa8, OLED_CMD);    //set multiplex display
    oled_write_byte(0x3f, OLED_CMD);    //1/64 duty
    oled_write_byte(0xd3, OLED_CMD);    //set display offset
    oled_write_byte(0x00, OLED_CMD);    //not offest
    oled_write_byte(0xd5, OLED_CMD);    //set display clock divide ratio/oscillator frequency
    oled_write_byte(0x80, OLED_CMD);    //set divide ratio 
    oled_write_byte(0xd9, OLED_CMD);    //set pre-charge period
    oled_write_byte(0xf1, OLED_CMD);    //pre-charge: 15 clocks, discharge: 1 clock
    oled_write_byte(0xda, OLED_CMD);    //set com pins hardware configuration 
    oled_write_byte(0x12, OLED_CMD);    //
    oled_write_byte(0xdb, OLED_CMD);    //set vcomh
    oled_write_byte(0x40, OLED_CMD);    //set vcom deselect level
    oled_write_byte(0x20, OLED_CMD);    //set page addressing mode
    oled_write_byte(0x02, OLED_CMD);    //
    oled_write_byte(0x8d, OLED_CMD);    //set charge pump enable/disable
    oled_write_byte(0x14, OLED_CMD);    //charge pump disable
    oled_write_byte(0xa4, OLED_CMD);    //disable entire dispaly on
    oled_write_byte(0xa6, OLED_CMD);    //disable inverse display on
    oled_write_byte(0xaf, OLED_CMD);    //turn on oled panel

    oled_write_byte(0xaf, OLED_CMD);    //display on

    oled_clear(Pen_Clear);
    oled_set_pos(0, 0);

}
#endif
