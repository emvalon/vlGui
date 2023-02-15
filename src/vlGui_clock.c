/**
 * @file vlGui_clock.c
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
#include <stdio.h>
#include "vlGui.h"
#include "stdio.h"
#include "vlGui_base.h"
#include "vlGui_fonts.h"
#include "vlGui_input.h"
#include "vlGui_clock.h"

static void 
vlGui_drawClock(struct vlGui_window_t *win, void *arg)
{
    char str[6];
    uint16_t x0, y0;
    struct vlGui_clock_t * clk;

    vlGui_windowClear(win);
    clk = (struct vlGui_clock_t *)win;
    x0 = win->win_width >> 1;
    y0 = win->win_height >> 1;

    vlGui_setFont(clk->font);
    /* Drw time */
    snprintf(str, sizeof(str), "%02d:%02d", clk->time.hour, clk->time.min);
    str[5] = '\0';
    vlGui_drawString(win, 0,0, str, VLGUI_COLOR_WHITE);

    /* Draw date */
    // snprintf(str, sizeof(str), "%04d", clk->date.year);

    vlGui_drawLine(win, x0, y0, 1, 1, 1, VLGUI_COLOR_WHITE);

    vlGui_drawCircle(win, x0, y0, (win->win_height >> 1) - 2);

}

int 
vlGui_clockProcessKey(struct vlGui_window_t *win, uint8_t key)
{
    return 0;
}

struct vlGui_clock_t * 
vlGui_clockCreate(struct vlGui_window_t *parent, int16_t x, int16_t y,     
                    int16_t width, uint16_t height)
{
    struct vlGui_clock_t * clk;

    VLGUI_ASSERT(parent);

    clk = (struct vlGui_clock_t *)vlGui_windowCreate(parent, x, y,
                            width, height, sizeof(*clk) - sizeof(*parent));
    if(!clk) {
        return clk;
    }

    clk->font = vlGui_wenquan_9pt;
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