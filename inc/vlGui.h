/**
 * @file vlGui.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-06
 * 
 * Copyright © 2021 - 2022 Weilong Shen (valonshen@foxmail.com)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */
#ifndef _VLGUI_H_
#define _VLGUI_H_
#include "vlGui_config.h"
#include "vlGui_window.h"
#include "vlGui_port.h"
#include "vlGui_base.h"
#include "vlGui_fonts.h"


#define VLGUI_COLOR_BLACK       (0)
#define VLGUI_COLOR_WHITE       (1)
#define VLGUI_COLOR_CONVERT     (2)

#define VLGUI_ASSERT(cons)  
#define VLGUI_TIME_GET(a, b)    ((int)(a - b) >= 0)
#define VLGUI_TIME_LET(a, b)    ((int)(a - b) <= 0)
#define VLGUI_TIME_GT(a, b)     ((int)(a - b) > 0)
#define VLGUI_TIME_LT(a, b)     ((int)(a - b) < 0)

#define VLGUI_MIN(a,b)          (a > b ? b : a)
#define VLGUI_MAX(a,b)          (a > b ? a : b)
#define VLGUI_UNUSED(x)         (void)x;
#define VLGUI_STR_CENTER_X(width, font, num)                    \
    ((width - (font->fontWidth * num)) >> 1)
#define VLGUI_STR_CENTER_Y(height, font, num)                   \
    ((height - (font->fontHeight * num)) >> 1)


typedef void (* displayInit_func_t)(uint8_t display);
typedef void (* displayFresh_func_t)(void);
typedef void (* drawBlock_func_t)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
typedef void (* drawPoint_func_t)(uint16_t x, uint16_t y, uint8_t color);
typedef void (* drawLineH_func_t)(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint8_t color);
typedef void (* drawLineV_func_t)(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint8_t color);
typedef void (* drawBitmap_func_t)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *bitmap);
typedef vlGui_color (* getPointColor_func_t)(uint16_t x, uint16_t y);

struct vlGui_driver_t {
    displayInit_func_t  pInit;
    displayFresh_func_t pFresh;
    drawBlock_func_t    pDrawBlock;
    drawPoint_func_t    pDrawPoint;
    drawLineH_func_t    pDrawLineH;
    drawLineV_func_t    pDrawLineV;
    drawBitmap_func_t   pDrawBitmap;
    getPointColor_func_t pGetPointColor;
};

struct vlGui_t {
    uint8_t refresh;
    int16_t width;
    int16_t height;
    const struct vlGui_font_t *curFont;
    struct vlGui_driver_t *displayDriver;
    vlGui_window_t *window;
};


extern struct vlGui_t *vlGui_cur_screen;


int vlGui_screen_init(struct vlGui_t *screen, int16_t width, int16_t height);

void vlGui_register_driver(struct vlGui_t *screen, struct vlGui_driver_t *driver);

void vlGui_turnOnOff(struct vlGui_t *screen, uint8_t display);

vlGui_window_t * vlinGui_getMainWindow(struct vlGui_t *screen);

void vlGui_refresh(void);

struct vlGui_driver_t * vlGui_portGetDriver(void);

void vlGui_setFont(const struct vlGui_font_t *font);
#endif