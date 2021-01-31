/**
  ******************************************************************************
  * @FileName               oled_ui.h
  * @Description            OLED class with menu functions
  * @author                 Liu Qi (purewhite@sjtu.edu.cn)
  * @note                   UNSTABLE VERSION!
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/
#ifndef __OLED_UI_H
#define __OLED_UI_H

#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
//#include "iwdg.h"
#include "adc.h"
#include <cmath>
//#include "dac.h"

//#include "BoardConfiguration.h"
//#include "RobotConfiguration.h"

#include "OLED.h"
#include <cstdarg>
#include <type_traits>
#include <initializer_list>
#include <stdio.h>

/**
 * 128*64 pixels. ~ASCII: 12*6; GB2312: 12*12
 * 128 = 4 + 12*10 + 4; 64 = 2 + 12*5 + 2
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
// Lines: [0, 11], 12, [13, 24], 25, [26, 37], 38, [39, 50], 51, [52, 63]
/***	MAP	***/
/***	CONSTANT VALUE	***/
#define ADC_CHANNALS (4)
#define OLED_ADC_CHANNAL (0)

#define COL_BASE_PIXEL 4    // pixels available: [4, 123]
//#define ROW_BASE_PIXEL 2    // pixels available: [2, 61]
#define MAX_COLUMN 20
#define MAX_ROW 5
#define CURSOR_BLINK_INTERVAL 20
#define CURSOR_UPDATE_INTERVAL 10
#define MENU_SIZE 16
const uint8_t textLineStartY[] = {0, 13, 26, 39, 52};
const uint8_t dividingLineY[] = {12, 25, 38, 51};
/***	CONSTANT VALUE	***/
class MenuTerm_Base;
class MenuTerm_Folder;
template <class T>
class MenuTerm_MutableParam;
template <class T>
class MenuTerm_ConstParam;
class MenuTerm_Text;

MenuTerm_Folder* createFolderTerm(const char* _text, std::initializer_list<MenuTerm_Base*> l);

template <class T>
MenuTerm_MutableParam<T>* createMutableTerm(const char *_preText, T *_param, const char *_postText, const T &_delta, const T &_lowerBound, const T &_upperBound, void (*_pressAction)() = nullptr);

template <class T>
MenuTerm_ConstParam<T>* createConstTerm(const char *_preText, const T *_param, const char *_postText);

MenuTerm_Text* createTextTerm(const char *_text);


/***	TYPE DEFINE	***/

class MenuTerm_Base{
public:
    virtual void Print(uint32_t line) const = 0;
    virtual void Pressed(uint32_t line) = 0;
    virtual void Left(uint32_t line) = 0;
    virtual void Right(uint32_t line) = 0;
};

class OLED_UI{
    friend class MenuTerm_Folder;

protected:
    static OLED device;
public:
    static OLED_UI oled_ui;

protected:
//    MenuTerm_Base *menu[MENU_SIZE];
    MenuTerm_Folder *menu;
    uint32_t currentCursorLine, menuStartLine;
//    uin32_t menuSize;

    void PrintASCII_Char(uint8_t x, uint8_t y, char chr);
    void PrintGB2312_Char(uint8_t x, uint8_t y, uint8_t chr1, uint8_t chr2);
    void PrintGB2312_Char(uint8_t x, uint8_t y, const char* chr);
    void PrintString(uint8_t x, uint8_t y, const char *str);
    void PrintString(const char *str);

public:
    OLED_UI() : menu(nullptr), currentCursorLine(0),  menuStartLine(0){}
    ~OLED_UI() = default;

    void Init();

    void LoadRootMenu(const char* _text, std::initializer_list<MenuTerm_Base*> l);
    void LoadRootMenu(MenuTerm_Folder* _menu) {menu = _menu;}

    void Update();
    void printf(uint8_t line, const char *fmt, ...);
    //void printf(const char *fmt, ...);
    void CursorMoveUp();
    void CursorMoveDown();
		
		void Refresh() { device.OLEDBurstRefresh();}
};

class MenuTerm_Folder: public MenuTerm_Base{
    friend MenuTerm_Folder* createFolderTerm(const char* _text, std::initializer_list<MenuTerm_Base*> l);
private:
    uint32_t size;
    const char* text;   // 作为菜单的一项显示的文字
    MenuTerm_Base *menuTerms[MENU_SIZE];
    MenuTerm_Folder *backwardsMenu;
public:
    MenuTerm_Folder(const char* _text, std::initializer_list<MenuTerm_Base*> l);

    uint32_t getSize() const {return size + ((backwardsMenu == nullptr) ? 0 : 1);}
    MenuTerm_Base* getTerm(uint32_t line) {
        if(backwardsMenu){
            if(line == 0) return backwardsMenu;
            else --line;
        }
		return menuTerms[line];

    }
    void Print(uint32_t line)  const override;
    void Pressed(uint32_t line) override;
    void Left(uint32_t) override {}
    void Right(uint32_t) override {}
    void Back() { OLED_UI::oled_ui.menu = backwardsMenu; }
};

template <class T>
class MenuTerm_MutableParam : public MenuTerm_Base{
    template <class D>
    friend MenuTerm_MutableParam<D>* createMutableTerm(const char *_preText, D *_param, const char *_postText, const D &_delta, const D &_lowerBound, const D &_upperBound, void (*_pressAction)());
private:
    const char *preText, *postText;
    T *param, delta, lowerBound, upperBound;
    void (*pressAction)();
public:
    MenuTerm_MutableParam(const char *_preText, T *_param, const char *_postText, const T &_delta, const T &_lowerBound, const T &_upperBound, void (*_pressAction)() = nullptr) : preText(_preText), postText(_postText), param(_param), delta(_delta), lowerBound(_lowerBound), upperBound(_upperBound), pressAction(_pressAction) {}
    void Print(uint32_t line)  const override{
        if(std::is_integral<T>::value){
            if(std::is_signed<T>::value)
                switch(sizeof(T)){
                    case 1: // int8_t
                    case 2: // int16_t
                    case 4: // int32_t
                        OLED_UI::oled_ui.printf(line, "%s%d%s", preText, (int32_t)(*param), postText);
                        break;
                    default:
                        Error_Handler();
                        break;
                }
            else
                switch(sizeof(T)){
                    case 1: // uint8_t
                    case 2: // uint16_t
                    case 4: // uint32_t
                        OLED_UI::oled_ui.printf(line, "%s%u%s", preText, (uint32_t)(*param), postText);
                        break;
                    default:
                        Error_Handler();
                        break;
                }
        }
        else if(std::is_floating_point<T>::value){
            switch(sizeof(T)){
                case 4: // float
                    OLED_UI::oled_ui.printf(line, "%s%f%s", preText, *param, postText);
                    break;
                case 8: // double
                    OLED_UI::oled_ui.printf(line, "%s%lf%s", preText, *param, postText);
                    break;
                default:
                    Error_Handler();
                    break;
            }
        }
    }
    void Pressed(uint32_t line) override {
        if(pressAction) pressAction();
    }
    void Left(uint32_t line) override{
        if(*param - delta >= lowerBound)
            *param -= delta;
    }
    void Right(uint32_t line) override{
        if(*param + delta <= upperBound)
            *param += delta;
    }
};

template <class T>
class MenuTerm_ConstParam : public MenuTerm_Base{
    template <class D>
    friend MenuTerm_ConstParam<D>* createConstTerm(const char *_preText, const D *_param, const char *_postText);
private:
    const char *preText, *postText;
    const T *param;
public:
    MenuTerm_ConstParam(const char *_preText, const T *_param, const char *_postText) : preText(_preText), postText(_postText), param(_param) {}
    void Print(uint32_t line)  const override{
        if(std::is_integral<T>::value){
            if(std::is_signed<T>::value)
                switch(sizeof(T)){
                    case 1: // int8_t
                    case 2: // int16_t
                    case 4: // int32_t
                        OLED_UI::oled_ui.printf(line, "%s%d%s", preText, *param, postText);
                        break;
                    default:
                        Error_Handler();
                        break;
                }
            else
                switch(sizeof(T)){
                    case 1: // uint8_t
                    case 2: // uint16_t
                    case 4: // uint32_t
                        OLED_UI::oled_ui.printf(line, "%s%u%s", preText, *param, postText);
                        break;
                    default:
                        Error_Handler();
                        break;
                }
        }
        else if(std::is_floating_point<T>::value){
            switch(sizeof(T)){
                case 4: // float
                    OLED_UI::oled_ui.printf(line, "%s%f%s", preText, *param, postText);
                    break;
                case 8: // double
                    OLED_UI::oled_ui.printf(line, "%s%lf%s", preText, *param, postText);
                    break;
                default:
                    Error_Handler();
                    break;
            }
        }
    }
    void Pressed(uint32_t line) override {}
    void Left(uint32_t) override {}
    void Right(uint32_t) override {}
};

class MenuTerm_Text : public MenuTerm_Base{
    friend MenuTerm_Text* createTextTerm(const char *_text);
//private:
public:
    const char *text;
public:
    explicit MenuTerm_Text(const char *_text) : text(_text) {}
    void Print(uint32_t line)  const override {OLED_UI::oled_ui.printf(line, text);}
    void Pressed(uint32_t line) override {}
    void Left(uint32_t line) override {}
    void Right(uint32_t line) override {}
};

// ---------------------

class OLED_Menu{
    private:
        OLED *device;
        int8_t currentCursorLine;

        //OLED_Menu *fatherMenu, *subMenu[MAX_ROW];

        void PrintASCII_Char(uint8_t x, uint8_t y, char chr);
        void PrintGB2312_Char(uint8_t x, uint8_t y, uint8_t chr1, uint8_t chr2);
        void PrintGB2312_Char(uint8_t x, uint8_t y, const char* chr);
        void PrintString(uint8_t x, uint8_t y, const char *chr);
        void PrintString(const char *chr);
        void printf(uint8_t x, uint8_t y, const char *fmt,...);

    public:
        static OLED_Menu root_menu;

        void (*PrintMenuText[MAX_ROW])(uint8_t);   // Called in Update() to Print the text of the given line
        void (*ButtonPressed[MAX_ROW])(uint8_t);    // Called in Update() if the corresponding button action is taken.
        void (*ButtonMoveLeft[MAX_ROW])(uint8_t);
        void (*ButtonMoveRight[MAX_ROW])(uint8_t);

        //OLED_Menu(GPIO_TypeDef *_DC_Port, uint16_t _DC_Pin, GPIO_TypeDef *_Rst_Port, uint16_t _Rst_Pin);
        OLED_Menu();
        ~OLED_Menu();
        void Init();
        void Update();
        void printf(uint8_t line, const char *fmt, ...);
        void printf(const char *fmt, ...);
        void CursorMoveUpDown(bool directionIsDown){
            //printf(1, directionIsDown ? "Down." : "Up.");
            if(directionIsDown){
                ++currentCursorLine;
                if(currentCursorLine >= 5)
                    currentCursorLine -= 5;
            }
            else{
                --currentCursorLine;
                if(currentCursorLine < 0)
                    currentCursorLine += 5;
            }
        }
};

/***	SPECIFIC INIT CONFIGURATION	***/

/***	EXTERNAL VARIABLES	***/
extern uint16_t ADC_Val[ADC_CHANNALS];
extern OLED_Menu *oled_menu;
/***	APIs	***/

extern const uint32_t objectPoolSize;
extern int8_t _OLED_POOL_objectPool[];
extern uint32_t _OLED_POOL_currentAddr;

void *operator new (size_t size,void *pbuffer);

template <class T>
MenuTerm_MutableParam<T>* createMutableTerm(const char *_preText, T *_param, const char *_postText, const T &_delta, const T &_lowerBound, const T &_upperBound, void (*_pressAction)()){
    MenuTerm_MutableParam<T>* ret = new ((void*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr)) MenuTerm_MutableParam<T>(_preText, _param, _postText, _delta, _lowerBound, _upperBound, _pressAction);
//    auto *ret = (MenuTerm_MutableParam<T>*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr);
    _OLED_POOL_currentAddr += sizeof(MenuTerm_MutableParam<T>);

//    ret->preText = _preText;
//    ret->postText = _postText;
//    ret->param = _param;
//    ret->delta = _delta;
//    ret->lowerBound = _lowerBound;
//    ret->upperBound = _upperBound;
//    ret->pressAction = _pressAction;
    return ret;
}

template <class T>
MenuTerm_ConstParam<T>* createConstTerm(const char *_preText, const T *_param, const char *_postText){
    MenuTerm_ConstParam<T>* ret = new ((void*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr)) MenuTerm_ConstParam<T>(_preText, _param, _postText);
//    auto *ret = (MenuTerm_ConstParam<T>*)(_OLED_POOL_objectPool+_OLED_POOL_currentAddr);
    _OLED_POOL_currentAddr += sizeof(MenuTerm_ConstParam<T>);

//    ret->preText = _preText;
//    ret->postText = _postText;
//    ret->param = _param;
    return ret;
}

#endif
