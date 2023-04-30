/**
 * @file vlGui_clock.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-28
 * 
 * Copyright ? 2021 - 2022 Weilong Shen (valonshen@foxmail.com)
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
#include <stdio.h>
#include <math.h>
#include "vlGui.h"
#include "stdio.h"
#include "vlGui_base.h"
#include "vlGui_fonts.h"
#include "vlGui_input.h"
#include "vlGui_clock.h"

const uint16_t ssss[] = {0xb6c0, 0xc0d1, 0xe8c9, 0xc3d6, 0x0000};
static void 
vlGui_drawClock(vlGui_window_t *win, void *arg)
{
    char str[6];
    uint8_t r;
    uint16_t x0, y0;
    struct vlGui_clock_t * clk;

    VLGUI_UNUSED(arg);
    clk = (struct vlGui_clock_t *)win;
    x0 = win->win_width >> 2;
    y0 = win->win_height >> 1;
    r = VLGUI_MIN(x0, y0) - 1;

    vlGui_setFont(vlGui_asc11x18);
    vlGui_windowClear(win);
    /* Drw time */
    // snprintf(str, sizeof(str), "%02d:%02d", clk->time.hour, clk->time.min);
    snprintf(str, sizeof(str), "%02d:%02d", 22, 44);

    str[5] = '\0';
    vlGui_drawString(win, 2 * x0 + 4, 8, str, VLGUI_COLOR_WHITE);
    vlGui_setFont(vlGui_wenquan_9pt);
    vlGui_drawString(win, 2 * x0 + 8, 26, (char *)"Æ½°²Ï²ÀÖ", VLGUI_COLOR_WHITE);

    vlGui_drawString(win, 2 * x0 + 8, 38, (char *)"ÐÒ¸£ÏàËæ", VLGUI_COLOR_WHITE);

    /* Draw date */
    r -= 4;
    vlGui_drawLine(win, x0, y0, x0 + r * cosf(clk->time.sec * 2 * 3.1415926 / 60.0), y0 + r * sinf(clk->time.sec * 2 * 3.1415926 / 60.0), 1, VLGUI_COLOR_WHITE);
    vlGui_drawCircle(win, x0, y0, r + 4);
    if (clk->time.sec >= 59) {
        clk->time.sec = 0;
    } else {
        // clk->time.sec++;
        clk->time.min++;
        clk->time.hour++;
    }
    vlGui_windowSetRefresh(win);
}

int 
vlGui_clockProcessKey(vlGui_window_t *win, uint8_t key)
{
    VLGUI_UNUSED(win);
    VLGUI_UNUSED(key);

    return 0;
}

struct vlGui_clock_t * 
vlGui_clockCreate(vlGui_window_t *parent, int16_t x, int16_t y,     
                  int16_t width, uint16_t height)
{
    struct vlGui_clock_t * clk;

    VLGUI_ASSERT(parent);

    clk = (struct vlGui_clock_t *)vlGui_windowCreate(parent, x, y,
                            width, height, sizeof(*clk) - sizeof(*parent));
    if(!clk) {
        return clk;
    }

    clk->font = vlGui_asc16x26;
    clk->date.year = 2022;
    clk->date.mon  = 1;
    clk->date.day  = 1;
    clk->time.min  = 0;
    clk->time.sec  = 0;

    vlGui_windowSetKeyCb(&clk->win, vlGui_clockProcessKey);
    vlGui_windowSetDrawCb(&clk->win, vlGui_drawClock);
    vlGui_windowSetRefresh(&clk->win);

    return clk;
}