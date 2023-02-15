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

struct vlGui_clock_date_t
{
    uint16_t year;
    uint8_t mon;
    uint8_t day;
};

struct vlGui_clock_time_t
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
};

struct vlGui_clock_t {
    struct vlGui_window_t win;
    struct vlGui_clock_date_t date;
    struct vlGui_clock_time_t time;
    const struct vlGui_font_t *font;
};


int vlGui_clockProcessKey(struct vlGui_window_t *win, uint8_t key);

struct vlGui_clock_t * vlGui_clockCreate(struct vlGui_window_t *parent, int16_t x, int16_t y,     
                                           int16_t width, uint16_t height);





#endif // _VLGUI_CLOCK_H_