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

#include "OLED.h"
#include "rtthread.h"

/**
 * @note Object of DJI OLED
 */
OLED OLED::_oled_device(&hspi1, OLED_DC_GPIO_Port, OLED_DC_Pin, OLED_RST_GPIO_Port, OLED_RST_Pin);

static rt_thread_t OLEDThread = RT_NULL;

/**
 * @brief Thread entry of OLED refresh
 * @param para No use
 * @note Multiscreen Application not supported
 * @return (None)
 */
void OLEDThreadEntry(void* para){
    while(1){
        OLED::_oled_device.OLEDRefresh();
        rt_thread_delay(5);
    }
}

/**
 * @brief Start the OLED refresh thread
 * @return (None)
 */
void OLED::RTThreadInit() {
    constexpr auto entry = &OLEDThreadEntry;
    OLEDThread = rt_thread_create("OLED",
                                  entry,
                                  RT_NULL,
                                  512,
                                  3,
                                  2);
    rt_thread_startup(OLEDThread);
}

/**
 * @brief Constructor of OLED object, define basic hardware information and initialize video memory
 * @param hspi SPI handler
 * @param _DC_Port DC GPIO Port
 * @param _DC_Pin DC GPIO Pin
 * @param _Rst_Port Rst GPIO Port
 * @param _Rst_Pin Rst GPIO Pin
 */
OLED::OLED(SPI_HandleTypeDef *hspi, GPIO_TypeDef *_DC_Port, uint16_t _DC_Pin, GPIO_TypeDef *_Rst_Port,
           uint16_t _Rst_Pin) : hspi(hspi), DC_Port(_DC_Port), Rst_Port(_Rst_Port), DC_Pin(_DC_Pin), Rst_Pin(_Rst_Pin) {
    for(int i = 0; i < 8; ++i) memset(oledBuffer[i], 0, 128 * sizeof(uint8_t));
}

/**
 * @brief Deconstructor of OLED object
 */
 OLED::~OLED() {
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

void OLED::Reset() {
    HAL_GPIO_WritePin(Rst_Port, Rst_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(Rst_Port, Rst_Pin, GPIO_PIN_SET);
}

/**
 * @brief Initialize the oled module
 * @param (None)
 * @return (None)
 */
void OLED::Init() {
    uint8_t initSerial[] = {
            0xae,   //turn off oled panel
            0x00,   //set low column address
            0x10,   //set high column address
            0x40,   //set start line address
            0x81,   //set contrast control resigter
            0xcf,   //set SEG output current brightness
            0xa1,   //set SEG/column mapping
    };

    Reset();
    OLEDWriteByte(0xae, OLED_CMD);    //turn off oled panel
    OLEDWriteByte(0x00, OLED_CMD);    //set low column address
    OLEDWriteByte(0x10, OLED_CMD);    //set high column address
    OLEDWriteByte(0x40, OLED_CMD);    //set start line address
    OLEDWriteByte(0x81, OLED_CMD);    //set contrast control resigter
    OLEDWriteByte(0x1f, OLED_CMD);    //set SEG output current brightness
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
    OLEDWriteByte(0x14, OLED_CMD);    //charge pump enable
    OLEDWriteByte(0xa4, OLED_CMD);    //disable entire dispaly on
    OLEDWriteByte(0xa6, OLED_CMD);    //disable inverse display on
    OLEDWriteByte(0xaf, OLED_CMD);    //turn on oled panel

    OLEDWriteByte(0xaf, OLED_CMD);    //display on

    FullScreenOperation(Pen_Clear);
    SetCursor(0, 0);

    RTThreadInit();
}

/**
 * @brief Turn on OLED display
 * @return (None)
 */
void OLED::DisplayOn() {
    OLEDWriteByte(0x8d, OLED_CMD);//set charge pump enable/disable
    OLEDWriteByte(0x14, OLED_CMD);//charge pump enable
    OLEDWriteByte(0xaf, OLED_CMD);//turn on oled panel
}

/**
 * @brief Turn off OLED display
 * @return (None)
 */
void OLED::DisplayOff() {
    OLEDWriteByte(0x8d, OLED_CMD);//set charge pump enable/disable
    OLEDWriteByte(0x10, OLED_CMD);//charge pump disable
    OLEDWriteByte(0xae, OLED_CMD);//turn off oled panel
}

/**
 * @brief Refresh OLED display by row
 * @return (None)
 */
void OLED::OLEDRefresh() {
    static uint8_t row = 0;

    SetCursor(0, row);
    CmdSet();
    HAL_SPI_Transmit(hspi, &(oledBuffer[row][0]), 2, 1000);
    HAL_SPI_Transmit_DMA(hspi, oledBuffer[row], 128);

    row++;
    row %= Y_WIDTH / 8;
}

/**
 * @brief Refresh the RAM of OLED
 * @return (None)
 */
void OLED::OLEDBurstRefresh() {
	
    for (uint8_t i = 0; i < 8; ++i){
        SetCursor(0, i);
        CmdSet();
        HAL_SPI_Transmit(hspi, &(oledBuffer[i][0]), 2, 1000);
        HAL_SPI_Transmit_DMA(hspi, oledBuffer[i], 128);
        HAL_Delay(10);
    }
}
