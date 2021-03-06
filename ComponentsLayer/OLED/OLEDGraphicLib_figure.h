//
// Created by bddwy on 2021/1/31.
//

#ifndef CYLINDERLOOP_OLEDGRAPHICLIB_FIGURE_H
#define CYLINDERLOOP_OLEDGRAPHICLIB_FIGURE_H

const unsigned char oledLib_Hero_invert[32][32] = {
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xEF, 0xAF, 0xEF},
        {0xAF, 0x2F, 0xCF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xE9, 0x89, 0x8B, 0x0B, 0x8B, 0x8B, 0x8B, 0x8B, 0x8B, 0xB8, 0xFA, 0xFA, 0xFB, 0x0A, 0x0B},
        {0x82, 0xA2, 0xAA, 0x2A, 0x0A, 0x80, 0xA3, 0xAB, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0xBE, 0xBE, 0xBE, 0xBE, 0x3E, 0x3E, 0x1E, 0x5E, 0x46, 0xF0, 0xF8, 0xE6},
        {0xF8, 0xE6, 0xC0, 0x58, 0x5E, 0x1E, 0x3E, 0x3E, 0xBE, 0xBE, 0xBF, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x1F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x0F, 0xAF, 0x8F, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0x01, 0x01, 0x11, 0x18, 0x18, 0x34, 0x20, 0x40, 0x00, 0x00, 0xAA, 0xFF, 0xFF, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0C},
        {0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFD, 0xFF, 0xFF, 0xAA, 0x00, 0x00, 0x00, 0x92, 0x92, 0xDB, 0xDB, 0xDB, 0xCB, 0x49, 0x01, 0x38, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0x3F, 0x8F, 0x4F, 0x5F, 0x03, 0x38, 0x38, 0x38, 0x1C, 0x06, 0x22, 0x33, 0x39, 0x19, 0x19, 0x19, 0x19, 0x39, 0x33, 0x23, 0x06, 0x1C, 0x08, 0x08, 0x28, 0x28, 0x28, 0x2C, 0x0C, 0x0C},
        {0x0D, 0x0D, 0x2C, 0x28, 0x28, 0x28, 0x08, 0x08, 0x1C, 0x06, 0x23, 0x33, 0x39, 0x19, 0x19, 0x19, 0x19, 0x39, 0x33, 0x22, 0x06, 0x1C, 0x38, 0x18, 0x18, 0x01, 0x5F, 0x4F, 0x8F, 0x3F, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFF, 0xFE, 0xFE, 0xFF, 0xFF, 0xF8, 0xE0, 0xC7, 0xCF, 0x9C, 0x98, 0x98, 0x98, 0x98, 0x9C, 0xCF, 0xC7, 0xE0, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xE0, 0xC7, 0xCF, 0x9C, 0x98, 0x98, 0x98, 0x98, 0x9C, 0xCF, 0xC7, 0xE0, 0xF8, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};

const unsigned char oledLib_DJILogo[128][8] = {
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x72},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x40,0x00,0x00,0x00,0x00,0x00,0x00,0xFE},
        {0x60,0x00,0x00,0x00,0x00,0x10,0x00,0x6C},
        {0x70,0x00,0x00,0x00,0x00,0x30,0x00,0x00},
        {0x78,0x00,0x00,0x00,0x00,0xF0,0x00,0x7C},
        {0x7C,0x00,0x00,0x00,0x07,0xF0,0x00,0xFE},
        {0x7E,0x00,0x00,0x00,0x3F,0xF0,0x00,0xC6},
        {0x7F,0x00,0x00,0x01,0xFF,0xF0,0x00,0xC6},
        {0x7F,0x80,0x00,0x0F,0xFF,0xF0,0x00,0xC6},
        {0x7F,0xC0,0x00,0x7F,0xFF,0xF0,0x00,0xFE},
        {0x7F,0xE0,0x03,0xFF,0xFF,0xF0,0x00,0x7C},
        {0x7F,0xF0,0x3F,0xFF,0xFF,0xF0,0x00,0x02},
        {0x7F,0xF8,0x3F,0xFF,0xFF,0xF0,0x00,0x06},
        {0x7F,0xFC,0x3F,0xFF,0xFF,0xF0,0x00,0x1E},
        {0x7F,0xFE,0x3F,0xFF,0xFF,0xF0,0x00,0xBC},
        {0x7F,0xFF,0x3F,0xFF,0xFF,0xF0,0x00,0xE0},
        {0x7F,0xFF,0xBF,0xFF,0xFF,0x80,0x00,0xF8},
        {0x7F,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x3E},
        {0x7F,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x0E},
        {0x7F,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xB8},
        {0x7F,0xFF,0xFF,0xF8,0x00,0x00,0x00,0xE0},
        {0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0xFE},
        {0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x1E},
        {0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x02},
        {0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00},
        {0x7F,0xEF,0xFF,0xF0,0x02,0x00,0x00,0x06},
        {0x7F,0xE7,0xFF,0xF0,0x02,0x00,0x00,0x0E},
        {0x7F,0xE3,0xFF,0xF0,0x02,0x00,0x00,0x1C},
        {0x7F,0xE1,0xFF,0xF8,0x03,0x00,0x00,0xBA},
        {0x7F,0xE0,0xFF,0xFC,0x03,0x00,0x00,0xF6},
        {0x7F,0xE0,0x7F,0xFE,0x03,0x80,0x00,0xE6},
        {0x7F,0xE0,0x3F,0xFF,0x03,0x80,0x00,0xF6},
        {0x7F,0xE0,0x3F,0xFF,0x83,0xC0,0x00,0x3E},
        {0x7F,0xE0,0x3F,0xFF,0xC3,0xC0,0x00,0x0E},
        {0x7F,0xE0,0x3F,0xFF,0xE3,0xE0,0x00,0x02},
        {0x7F,0xE0,0x3F,0xFF,0xF3,0xE0,0x00,0x00},
        {0x7F,0xE0,0x3F,0xFF,0xFB,0xF0,0x00,0x02},
        {0x7F,0xE0,0x3F,0xFF,0xFF,0xF0,0x00,0x66},
        {0x7F,0xE0,0x3F,0xFF,0xFF,0xF8,0x00,0xF6},
        {0x7F,0xE0,0x3F,0xFF,0xFF,0xF8,0x00,0xD6},
        {0x7F,0xE0,0x3F,0xFF,0xFF,0xFC,0x00,0xD6},
        {0x7F,0xF0,0x7F,0xFF,0xFF,0xFC,0x00,0xD6},
        {0x7F,0xF8,0xFF,0xF7,0xFF,0xFE,0x00,0xD6},
        {0x7F,0xFF,0xFF,0xF3,0xFF,0xFE,0x00,0xDE},
        {0x3F,0xFF,0xFF,0xE1,0xFF,0xFF,0x00,0x8C},
        {0x3F,0xFF,0xFF,0xE0,0xFF,0xCF,0x00,0x40},
        {0x1F,0xFF,0xFF,0xC0,0x7F,0xC7,0x80,0xC0},
        {0x1F,0xFF,0xFF,0xC0,0x3F,0xC3,0x80,0xC0},
        {0x0F,0xFF,0xFF,0x80,0x1F,0xC1,0xC0,0xFE},
        {0x07,0xFF,0xFF,0x00,0x0F,0xC0,0xC0,0xFE},
        {0x03,0xFF,0xFE,0x00,0x07,0xC0,0x60,0xC0},
        {0x01,0xFF,0xFC,0x00,0x03,0xC0,0x20,0xC0},
        {0x00,0x7F,0xF0,0x00,0x01,0xC0,0x00,0x86},
        {0x00,0x0F,0x80,0x00,0x00,0xC0,0x00,0x16},
        {0x00,0x00,0x00,0x00,0x00,0x40,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD0},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x72},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x62},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD6},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDE},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8C},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

#endif //CYLINDERLOOP_OLEDGRAPHICLIB_FIGURE_H
