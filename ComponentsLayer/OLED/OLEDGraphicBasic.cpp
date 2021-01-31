#include "OLED.h"

/**
 * @brief Clear, write or invert the hole screen
 * @return (None)
 */
void OLED::FullScreenOperation(Pen_e pen) {
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
