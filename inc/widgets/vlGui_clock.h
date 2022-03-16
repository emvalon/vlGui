/**
 * @file vlGui_clock.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-28
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
#ifndef _VLGUI_CLOCK_H_
#define _VLGUI_CLOCK_H_

#include <stdint.h>
#include "vlGui_window.h"
#include "vlGui_input.h"
#include "vlGui_fonts.h"

struct vlGui_clockDate {
    uint16_t year;
    uint8_t mon;
    uint8_t day;
};
typedef struct vlGui_clockDate vlGui_clockDate_t;

struct vlGui_clockTime {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
};
typedef struct vlGui_clockTime vlGui_clockTime_t;

struct vlGui_clockData {
    vlGui_clockDate_t date;
    vlGui_clockTime_t time;
    const struct vlGui_font_t *clockFont;
    const struct vlGui_font_t *strFont;
    uint32_t nextSecPoint;
    uint8_t week;
    uint8_t weather;
};
typedef struct vlGui_clockData vlGui_clockData_t;

struct vlGui_clockWin {
    vlGui_window_t win;
    vlGui_clockData_t data;
};
typedef struct vlGui_clockWin vlGui_clockWin_t;

void vlGui_clockDrawWin(vlGui_window_t *win, vlGui_clockData_t *data, uint8_t arg);

int vlGui_clockProcessKey(vlGui_window_t *win, uint8_t key);

void vlGui_clockInitData(vlGui_window_t *win, vlGui_clockData_t *data);

vlGui_clockWin_t *vlGui_clockCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width,
                                    uint16_t height);

void vlGui_clockSetTime(vlGui_clockData_t *clk, vlGui_clockTime_t *time);

#endif // _VLGUI_CLOCK_H_