/**
  ******************************************************************************
  * @FileName               oled_ui.cpp
  * @Description            Implementation of oled_ui.h
  * @author                 Liu Qi (purewhite@sjtu.edu.cn)
  * @note                   UNSTABLE VERSION!
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include "oled_ui.h"
OLED OLED_UI::device;
OLED_UI OLED_UI::oled_ui;


OLED_Menu OLED_Menu::root_menu;
OLED_Menu *oled_menu = &OLED_Menu::root_menu;

uint16_t ADC_Val[ADC_CHANNALS];

extern int8_t _OLED_POOL_objectPool[];
uint32_t _OLED_POOL_currentAddr = 0;

void *operator new (size_t size,void *pbuffer)
{
    size;
    return pbuffer;
}

MenuTerm_Folder* createFolderTerm(const char* _text, std::initializer_list<MenuTerm_Base*> l){
    MenuTerm_Folder* ret = new ((void*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr)) MenuTerm_Folder(_text, l);

//    MenuTerm_Folder* ret = (MenuTerm_Folder*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr);
//    ret->MenuTerm_Folder(_text, l);
    _OLED_POOL_currentAddr += sizeof(MenuTerm_Folder);
//    auto arg = l.begin();
//
//    ret->text = _text;
//    ret->size = l.size();
//    ret->backwardsMenu = nullptr;
//
//    for(uint32_t i = 0; i < l.size(); ++i){
//        ret->menuTerms[i] = *arg;
//        ++arg;
//    }

    return ret;
}

MenuTerm_Text* createTextTerm(const char *_text){
//    MenuTerm_Text* ret = (MenuTerm_Text*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr);
//    _OLED_POOL_currentAddr += sizeof(MenuTerm_Text);
//    ret->text = _text;
//
//
//    OLED_UI::oled_ui.printf(0, ret->text);
//
//    (*ret).Print(1);
//    int *t = new (_OLED_POOL_objectPool) int(1);
    MenuTerm_Text* ret = new((void*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr)) MenuTerm_Text(_text);
    _OLED_POOL_currentAddr += sizeof(MenuTerm_Text);
    return ret;
}

void OLED_UI::PrintASCII_Char(uint8_t x, uint8_t y, char chr){
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr -= ' ';

    for (t = 0; t < 12; ++t){
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; ++t1){
            if(temp&0x80u)
                OLED_UI::device.DrawPoint(x, y, Pen_Write);
            else
                OLED_UI::device.DrawPoint(x, y, Pen_Clear);

            temp <<= 1u;
            ++y;
            if((y - y0) == 12){
                y = y0;
                ++x;
                break;
            }
        }
    }
}
void OLED_UI::PrintGB2312_Char(uint8_t x, uint8_t y, uint8_t chr1, uint8_t chr2){
    uint8_t buffer[24];
    uint32_t offset = (94*(uint32_t)(chr1-0xa0-1) + (chr2-0xa0-1))*24;
    memcpy(buffer, gb2312_1212+offset, 24*sizeof(uint8_t));
    for(int i = 0; i < 12; ++i){
        for(int j = 0; j < 8; ++j){
            if(buffer[i*2] & PowOf2[j])
                OLED_UI::device.DrawPoint(x, y, Pen_Write);
            else
                OLED_UI::device.DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        for(int j = 0; j < 4; ++j){
            if(buffer[i*2+1] & PowOf2[j])
                OLED_UI::device.DrawPoint(x, y, Pen_Write);
            else
                OLED_UI::device.DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        ++y;
        x -= 12;
    }
}
void OLED_UI::PrintGB2312_Char(uint8_t x, uint8_t y, const char* chr){
    uint8_t buffer[24];
    uint32_t offset = (94*(uint32_t)(chr[0]-0xa0-1) + (chr[1]-0xa0-1))*24;
    memcpy(buffer, gb2312_1212+offset, 24*sizeof(uint8_t));
    for(int i = 0; i < 12; ++i){
        for(int j = 0; j < 8; ++j){
            if(buffer[i*2] & PowOf2[j])
                OLED_UI::device.DrawPoint(x, y, Pen_Write);
            else
                OLED_UI::device.DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        for(int j = 0; j < 4; ++j){
            if(buffer[i*2+1] & PowOf2[j])
                OLED_UI::device.DrawPoint(x, y, Pen_Write);
            else
                OLED_UI::device.DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        ++y;
        x -= 12;
    }
}
void OLED_UI::PrintString(uint8_t x, uint8_t y, const char *str){
    for(uint8_t i = 0; str[i] != '\0'; ++i){
        if((signed char)str[i] > 0){     // ASCII
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
                OLED_UI::device.Error("PrintString(): Invalid coordinate for ASCII Characters.\n", 4);
                return;
            }
            if(str[i] == '\n'){
                y += 12;
                x = COL_BASE_PIXEL;
                continue;
            }
            PrintASCII_Char(x, y, str[i]);
            x += 6;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
                OLED_UI::device.Error("PrintString(): String too long.\n", 5);
                return;
//                x = COL_BASE_PIXEL;
//                y += 12;
            }
        }
        else{               // GB2312
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
                OLED_UI::device.Error("PrintString(): Invalid coordinate for GB2312 Characters.\n", 4);
                return;
            }
            PrintGB2312_Char(x, y, str[i], str[i+1]);
            ++i;
            x += 12;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
                OLED_UI::device.Error("PrintString(): String too long.\n", 5);
                return;
//                x = COL_BASE_PIXEL;
//                y += 12;
            }
        }
    }
}
void OLED_UI::PrintString(const char *str){
    uint8_t x = COL_BASE_PIXEL, y = 0;
    for(uint8_t i = 0; str[i] != '\0'; ++i){
        if((signed char)str[i] > 0){     // ASCII
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
                OLED_UI::device.Error("PrintString(): Invalid coordinate for ASCII Characters.\n", 4);
                return;
            }
            if(str[i] == '\n'){
                y += 12;
                x = COL_BASE_PIXEL;
                continue;
            }
            PrintASCII_Char(x, y, str[i]);
            x += 6;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
                x = COL_BASE_PIXEL;
                y += 12;
            }
        }
        else{               // GB2312
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
                OLED_UI::device.Error("PrintString(): Invalid coordinate for GB2312 Characters.\n", 4);
                return;
            }
            PrintGB2312_Char(x, y, str[i], str[i+1]);
            ++i;
            x += 12;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
                x = COL_BASE_PIXEL;
                y += 12;
            }
        }
    }
    //OLED_UI::device.RefreshGram();
}

void OLED_UI::Init(){
    OLED_UI::device.Init();
    memset(ADC_Val, 0, sizeof(ADC_Val));
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Val, ADC_CHANNALS);
    OLED_UI::device.Clear(Pen_Clear);
    OLED_UI::device.RefreshGram();
}
void OLED_UI::LoadRootMenu(const char* _text, std::initializer_list<MenuTerm_Base*> l){
    menu = createFolderTerm(_text, l);
}

void OLED_UI::CursorMoveUp(){
    if(currentCursorLine > 0) {
        if(currentCursorLine == menuStartLine)
            --menuStartLine;
        --currentCursorLine;
    }
}
void OLED_UI::CursorMoveDown(){
    if(currentCursorLine < menu->getSize()-1) {
        if(currentCursorLine == menuStartLine + 4)
            ++menuStartLine;
        ++currentCursorLine;
    }
}

void OLED_UI::Update(){
	if(OLED_UI::oled_ui.menu == nullptr) return;
    static uint8_t cntForScreenUpdate = 0, cntForCursorUpdate = 0, lastButtonState = 0;
    uint32_t buttonVoltage = 0;
    ++cntForScreenUpdate;
    ++cntForCursorUpdate;
    if(cntForCursorUpdate >= CURSOR_UPDATE_INTERVAL) {
        OLED_UI::device.Clear(Pen_Clear);
//        if((int32_t)currentCursorLine + 5 > int32_t(menu->getSize())){
//            menuStartLine = int32_t(menu->getSize()) - 5;
//            menuEndLine = int32_t(menu->getSize());
//        }
//        else{
//            menuStartLine = currentCursorLine;
//            menuEndLine = currentCursorLine + 5;
//        }

        for(uint8_t i = menuStartLine; i < menuStartLine+5; ++i){
            if(menu->getTerm(i) != nullptr)
                menu->getTerm(i)->Print(i-menuStartLine);
        }
        buttonVoltage = ADC_Val[OLED_ADC_CHANNAL];
        if(buttonVoltage / 700 != lastButtonState)
            switch(lastButtonState = buttonVoltage / 700) {
                case 0:         // Pressed
                    menu->getTerm(currentCursorLine)->Pressed(currentCursorLine);
                    break;
                case 1:         // Left
                    menu->getTerm(currentCursorLine)->Left(currentCursorLine);
                    break;
                case 2:         // Right
                    menu->getTerm(currentCursorLine)->Right(currentCursorLine);
                    break;
                case 3:         // Up
                    CursorMoveUp();
                    break;
                case 4:         // Down
                    CursorMoveDown();
                    break;
                default:
                    // printf(1, "Nothing happened.");
                    break;
            }
        //printf(0, "%d", buttonVoltage);
        cntForCursorUpdate = 0;
    }

    if(cntForScreenUpdate >= CURSOR_BLINK_INTERVAL){
        OLED_UI::device.Invert(0, textLineStartY[currentCursorLine-menuStartLine], 127, 12);
        cntForScreenUpdate = 0;
    }
    for(uint8_t i = 0; i < 4; ++i)
        OLED_UI::device.DrawLine(0, dividingLineY[i], 127, dividingLineY[i], Pen_Write);
    OLED_UI::device.RefreshGram();
}

void OLED_UI::printf(uint8_t line, const char *fmt, ...){
    char OLED_Buffer[128] = {0};
    va_list ap;

    if(line > 4){
        OLED_UI::device.Error("printf(): Invalid coordinate.\n", 1);
        return;
    }

    va_start(ap, fmt);
    vsprintf((char *)OLED_Buffer, fmt, ap);
    va_end(ap);
    OLED_UI::device.Clear(0, textLineStartY[line], 127, 12);
    PrintString(COL_BASE_PIXEL, textLineStartY[line], OLED_Buffer);
}


MenuTerm_Folder::MenuTerm_Folder(const char* _text, std::initializer_list<MenuTerm_Base*> l) : size(l.size() > MENU_SIZE ? MENU_SIZE : l.size()), text(_text), menuTerms{nullptr}, backwardsMenu(nullptr) {
    memset(menuTerms, 0, MENU_SIZE * sizeof(MenuTerm_Base*));
    auto args = l.begin();
    for(uint32_t i = 0; i < l.size(); ++i) {
        menuTerms[i] = *args;
        ++args;
    }
}

void MenuTerm_Folder::Print(uint32_t line) const{
    OLED_UI::oled_ui.printf(line, text);
}

void MenuTerm_Folder::Pressed(uint32_t line) {
    backwardsMenu = OLED_UI::oled_ui.menu;
    OLED_UI::oled_ui.menu = this;
    OLED_UI::oled_ui.currentCursorLine = 0;
    OLED_UI::oled_ui.menuStartLine = 0;
//    if(backwardsMenu)
//        backwardsMenu->menuTerms = OLED_UI::oled_ui.menu;
//    OLED_UI::oled_ui.menu = this;
//
//    if(backwardsMenu)
//        backwardsMenu->menuTerms = menuTerms;
//    OLED_UI::oled_ui.menuTerms = menuTerms;
}

// ----------------------------------------------------


/**
 * @brief Constructor of the OLED_Menu class
 * @param *: As its name indicates.
 * @return (None)
 */
OLED_Menu::OLED_Menu() :device(&OLED::_oled_device), currentCursorLine(0){
    // OLED(_DC_Port, _DC_Pin, _Rst_Port, _Rst_Pin);
    for(int i = 0; i < 128; ++i)
        memset(gram[i], 0, 8*sizeof(uint8_t));
    memset(PrintMenuText, 0, sizeof(PrintMenuText));
    memset(ButtonPressed, 0, sizeof(ButtonPressed));
    memset(ButtonMoveLeft, 0, sizeof(ButtonMoveLeft));
    memset(ButtonMoveRight, 0, sizeof(ButtonMoveRight));
    //memset(subMenu, 0, sizeof(subMenu));

}

/**
 * @brief Destructor of the OLED_Menu class
 * @param *: As its name indicates.
 * @return (None)
 */
OLED_Menu::~OLED_Menu(){
		device->Clear(Pen_Clear);
		device->RefreshGram();
		device->DisplayOff();
}

/**
 * @brief Print an ASCII Character of 12x6 at a given position
 * @param x,y: Position of the upper-left point of the character
 * @param chr: The character to be put
 * @return (None)
 */
void OLED_Menu::PrintASCII_Char(uint8_t x, uint8_t y, char chr){
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr -= ' ';

    for (t = 0; t < 12; ++t){
        temp = asc2_1206[chr][t];

        for (t1 = 0; t1 < 8; ++t1){
            if(temp&0x80u)
                device->DrawPoint(x, y, Pen_Write);
            else
                device->DrawPoint(x, y, Pen_Clear);

            temp <<= 1u;
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
 * @brief Print an GB2312 Character of 12x6 at a given position
 * @param x,y: Position of the upper-left point of the character
 * @param chr1,chr2: The character to be put
 * @return (None)
 */
void OLED_Menu::PrintGB2312_Char(uint8_t x, uint8_t y, uint8_t chr1, uint8_t chr2){
    uint8_t buffer[24];
    uint32_t offset = (94*(uint32_t)(chr1-0xa0-1) + (chr2-0xa0-1))*24;
    memcpy(buffer, gb2312_1212+offset, 24*sizeof(uint8_t));
    for(int i = 0; i < 12; ++i){
        for(int j = 0; j < 8; ++j){
            if(buffer[i*2] & PowOf2[j])
                device->DrawPoint(x, y, Pen_Write);
            else
                device->DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        for(int j = 0; j < 4; ++j){
            if(buffer[i*2+1] & PowOf2[j])
                device->DrawPoint(x, y, Pen_Write);
            else
                device->DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        ++y;
        x -= 12;
    }
}

/**
 * @brief Print an GB2312 Character of 12x12 at a given position
 * @param x,y: Position of the upper-left point of the character
 * @param chr: The character to be put
 * @return (None)
 */
void OLED_Menu::PrintGB2312_Char(uint8_t x, uint8_t y, const char* chr){
    uint8_t buffer[24];
    uint32_t offset = (94*(uint32_t)(chr[0]-0xa0-1) + (chr[1]-0xa0-1))*24;
    memcpy(buffer, gb2312_1212+offset, 24*sizeof(uint8_t));
    for(int i = 0; i < 12; ++i){
        for(int j = 0; j < 8; ++j){
            if(buffer[i*2] & PowOf2[j])
                device->DrawPoint(x, y, Pen_Write);
            else
                device->DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        for(int j = 0; j < 4; ++j){
            if(buffer[i*2+1] & PowOf2[j])
                device->DrawPoint(x, y, Pen_Write);
            else
                device->DrawPoint(x, y, Pen_Clear);
            ++x;
        }
        ++y;
        x -= 12;
    }
}

/**
 * @brief Print a string(Chinese supported) at the whole screen and refresh the gram immediately.
 * @param str: The string to be put
 * @return (None)
 */
void OLED_Menu::PrintString(const char *str){
		uint8_t x = COL_BASE_PIXEL, y = 0;
    for(uint8_t i = 0; str[i] != '\0'; ++i){
        if((signed char)str[i] > 0){     // ASCII
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
				device->Error("PrintString(): Invalid coordinate for ASCII Characters.\n", 4);
				return;
			}
			if(str[i] == '\n'){
					y += 12;
					x = COL_BASE_PIXEL;
					continue;
			}
            PrintASCII_Char(x, y, str[i]);
            x += 6;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
                x = COL_BASE_PIXEL;
                y += 12;
            }
        }
        else{               // GB2312
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
					device->Error("PrintString(): Invalid coordinate for GB2312 Characters.\n", 4);
					return;
			}
            PrintGB2312_Char(x, y, str[i], str[i+1]);
            ++i;
            x += 12;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
                x = COL_BASE_PIXEL;
                y += 12;
            }
        }
    }
		//device->RefreshGram();
}

/**
 * @brief Print a string(Chinese supported) at a given position
 * @param x,y: Position of the upper-left point of the character
 * @param str: The string to be put
 * @return (None)
 */
void OLED_Menu::PrintString(uint8_t x, uint8_t y, const char *str){
    for(uint8_t i = 0; str[i] != '\0'; ++i){
        if((signed char)str[i] > 0){     // ASCII
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
				device->Error("PrintString(): Invalid coordinate for ASCII Characters.\n", 4);
				return;
			}
			if(str[i] == '\n'){
					y += 12;
					x = COL_BASE_PIXEL;
					continue;
			}
            PrintASCII_Char(x, y, str[i]);
            x += 6;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_ASCII)){
							device->Error("PrintString(): String too long.\n", 5);
							return;
//                x = COL_BASE_PIXEL;
//                y += 12;
            }
        }
        else{               // GB2312
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
					device->Error("PrintString(): Invalid coordinate for GB2312 Characters.\n", 4);
					return;
			}
            PrintGB2312_Char(x, y, str[i], str[i+1]);
            ++i;
            x += 12;
            if(x > (X_WIDTH - COL_BASE_PIXEL - CHAR_SIZE_WIDTH_GB2312)){
									device->Error("PrintString(): String too long.\n", 5);
									return;
//                x = COL_BASE_PIXEL;
//                y += 12;
            }
        }
    }
}

/**
 * @brief Initialize the OLED Menu class
 * @param (None)
 * @return (None)
 */
void OLED_Menu::Init(){
	device->Init();
	memset(ADC_Val, 0, sizeof(ADC_Val));
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Val, ADC_CHANNALS);
	device->Clear(Pen_Clear);
    device->RefreshGram();
}

/**
 * @brief Refresh the OLED screen and update the position of cursor
 * @param (None)
 * @return (None)
 */
void OLED_Menu::Update(){
    static uint8_t cntForScreenUpdate = 0, cntForCursorUpdate = 0, lastButtonState = 0;
    uint32_t buttonVoltage = 0;
    ++cntForScreenUpdate;
    ++cntForCursorUpdate;
    if(cntForCursorUpdate >= CURSOR_UPDATE_INTERVAL) {
			device->Clear(Pen_Clear);
			for(uint8_t i = 0; i < MAX_ROW; ++i){
					if(PrintMenuText[i]) PrintMenuText[i](i);
			}
			buttonVoltage = ADC_Val[OLED_ADC_CHANNAL];
				if(buttonVoltage / 700 != lastButtonState)
					switch(lastButtonState = buttonVoltage / 700) {
							case 0:         // Pressed
									if(ButtonPressed[currentCursorLine] != nullptr) ButtonPressed[currentCursorLine](currentCursorLine);
									break;
							case 1:         // Left
									if(ButtonMoveLeft[currentCursorLine] != nullptr) ButtonMoveLeft[currentCursorLine](currentCursorLine);
									break;
							case 2:         // Right
									if(ButtonMoveRight[currentCursorLine] != nullptr) ButtonMoveRight[currentCursorLine](currentCursorLine);
									break;
							case 3:         // Up
									CursorMoveUpDown(false);
									break;
							case 4:         // Down
									CursorMoveUpDown(true);
									break;
							default:
									// printf(1, "Nothing happened.");
									break;
					}
				//printf(0, "%d", buttonVoltage);
        cntForCursorUpdate = 0;	
    }

    if(cntForScreenUpdate >= CURSOR_BLINK_INTERVAL){
        device->Invert(0, textLineStartY[currentCursorLine], 127, 12);
        cntForScreenUpdate = 0;
    }
    for(uint8_t i = 0; i < 4; ++i)
        device->DrawLine(0, dividingLineY[i], 127, dividingLineY[i], Pen_Write);
    device->RefreshGram();
}

/**
 * @brief Format and print a string(Chinese supported) at the whole screen
 * @param *fmt,... The format of the string
 * @return (None)
 */
void OLED_Menu::printf(const char *fmt, ...){
    char OLED_Buffer[128] = {0};
    va_list ap;

    va_start(ap, fmt);
    vsprintf((char *)OLED_Buffer, fmt, ap);
    va_end(ap);
		for(uint8_t i = 0; i < 5; ++i)
			device->Clear(0, textLineStartY[i], 127, 12);
    PrintString(OLED_Buffer);
}

/**
 * @brief Format and print a string(Chinese supported) at a given line
 * @param line The line you want to output the string in
 * @param *fmt,... The format of the string
 * @return (None)
 */
void OLED_Menu::printf(uint8_t line, const char *fmt, ...){
    char OLED_Buffer[128] = {0};
    va_list ap;

    if(line > 4){
        device->Error("printf(): Invalid coordinate.\n", 1);
	    return;
	}

    va_start(ap, fmt);
    vsprintf((char *)OLED_Buffer, fmt, ap);
    va_end(ap);
    device->Clear(0, textLineStartY[line], 127, 12);
    PrintString(COL_BASE_PIXEL, textLineStartY[line], OLED_Buffer);
}
