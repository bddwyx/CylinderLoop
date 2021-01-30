#include "oled.h"
#include <stdio.h>
#include <stdarg.h>

/**
 * @brief Print a ascii character
 * @param row: row of the character
 * @param col: column of the character
 * @param chr: the character ready to print
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
 * @param fmt: the pointer to format character string
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