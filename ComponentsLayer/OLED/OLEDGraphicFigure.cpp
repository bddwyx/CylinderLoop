#include "oled.h"

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