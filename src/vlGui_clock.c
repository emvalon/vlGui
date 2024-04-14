/**
 * @file vlGui_clock.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-26
 * 
 * Copyright 2021 - 2023 Weilong Shen (valonshen@foxmail.com)
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
#include <string.h>
#include "vlGui.h"
#include "vlGui_base.h"
#include "vlGui_clock.h"

#define VLGUI_CLOCK_UPDATE_NONE                 (0)
#define VLGUI_CLOCK_UPDATE_SEPARATOR            (1)
#define VLGUI_CLOCK_UPDATE_TIMER                (2)
#define VLGUI_CLOCK_UPDATE_ALL                  (3)

static const uint16_t strWeek_cn[] = {
    0xD0C7, 0xC6DA, 0
};
static const uint16_t strNumber_cn[][2] = {
    {0xD2BB, 0}, {0xB6FE, 0}, {0xC8FD, 0}, {0xCBC4, 0}, 
    {0xCEE5, 0}, {0xC1F9, 0}, {0xC8D5, 0}
};
static const uint16_t strWeather_cn[][2] = {
   {0xC7E7, 0}, {0xD2F5, 0}, {0xD4C6, 0}, {0xD3EA, 0},
   {0xCEED, 0},
};

static uint8_t
vlGui_clockElapsed(struct vlGui_clock_t *clk, uint8_t sec)
{
    struct vlGui_clock_time_t *time = &clk->time;
    struct vlGui_clock_date_t *date = &clk->date;

    time->sec += sec;
    if (time->sec >= 60) {
        ++time->min;
        time->sec -= 60;
    } else {
        return VLGUI_CLOCK_UPDATE_SEPARATOR;
    }

    if (time->min >= 60) {
        ++time->hour;
        time->min -= 60;
    } else {
        return VLGUI_CLOCK_UPDATE_TIMER;
    }

    if (time->hour >= 24) {
        ++date->day;
        clk->week = (clk->week + 1) % 7;
        time->hour -= 24;
    } else {
        return VLGUI_CLOCK_UPDATE_TIMER;
    }

    /* TODO: Check if need to update month and year. */
    return VLGUI_CLOCK_UPDATE_ALL;
}

static void 
vlGui_drawClock(vlGui_window_t *win, uint8_t arg)
{
    char str[6];
    int16_t x, y;
    uint8_t update;
    uint32_t now;
    struct vlGui_clock_t *clk;
    const struct vlGui_font_t *strFont, *clkFont;

    update = VLGUI_CLOCK_UPDATE_NONE;
    clk = (struct vlGui_clock_t *)win;
    strFont = clk->strFont;
    clkFont = clk->clockFont;
    now = vlGui_getTimeMs();
    if (VLGUI_TIME_GET(now, clk->nextSecPoint)) {
        clk->nextSecPoint = now + 1000;
        update = vlGui_clockElapsed(clk, 1);
    }
    if ((arg == VLGUI_WIN_DRAW_BACKGROUND) ||
        ((arg != VLGUI_WIN_DRAW_INIT) && (update == VLGUI_CLOCK_UPDATE_NONE))) {
        return;
    }

    x = VLGUI_STR_CENTER_X(win->win_width, clkFont, 5);
    y = VLGUI_STR_CENTER_Y(win->win_height, clkFont, 2);
    if ((arg == VLGUI_WIN_DRAW_INIT) || (update == VLGUI_CLOCK_UPDATE_ALL)) {
        vlGui_windowClear(win);
        vlGui_setFont(strFont);
        y += clkFont->fontHeight;
        vlGui_drawString(win, x, y, (char *)strWeek_cn, VLGUI_COLOR_WHITE);
        vlGui_drawString(win, x + (2 * strFont->fontWidth), y, 
                         (char *)strNumber_cn[clk->week], VLGUI_COLOR_WHITE);
        
        vlGui_drawString(win, x + (5 * clkFont->fontWidth) - strFont->fontWidth, 
                         y, (char *)strWeather_cn[clk->weather], 
                         VLGUI_COLOR_WHITE);
        y = VLGUI_STR_CENTER_Y(win->win_height, clkFont, 2);
    }

    /* Draw the time number */
    if ((arg == VLGUI_WIN_DRAW_INIT) || 
        (update != VLGUI_CLOCK_UPDATE_SEPARATOR)) {
        vlGui_drawBlock(win, x, y, 5 * clkFont->fontWidth, clkFont->fontHeight, 
                        VLGUI_COLOR_BLACK);

        vlGui_setFont(clkFont);
        snprintf(str, sizeof(str), "%02d %02d", clk->time.hour % 24, clk->time.min % 60);
        str[5] = '\0';
        vlGui_drawString(win, x, y, str, VLGUI_COLOR_WHITE);

    } else {
        vlGui_drawBlock(win, x + (2 * clkFont->fontWidth), y, clkFont->fontWidth,
                        clkFont->fontHeight, VLGUI_COLOR_BLACK);
    }

    if (clk->showSeparator) {
        vlGui_drawString(win, x + (2 * clkFont->fontWidth), y, ":", 
                         VLGUI_COLOR_WHITE);
    }
    clk->showSeparator ^= 1;

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

    clk->clockFont = vlGui_asc11x18;
    clk->strFont = vlGui_wenquan_9pt;
    clk->date.year = 2022;
    clk->date.mon  = 1;
    clk->date.day  = 1;
    clk->time.hour = 0;
    clk->time.min  = 0;
    clk->time.sec  = 0;
    clk->week = 0;
    clk->weather = 0;
    clk->flags = 0;
    clk->nextSecPoint = vlGui_getTimeMs() + 1000;

    vlGui_windowSetKeyCb(&clk->win, vlGui_clockProcessKey);
    vlGui_windowSetDrawCb(&clk->win, vlGui_drawClock);
    vlGui_windowSetRefresh(&clk->win);
    vlGui_windowBackgroundUpdate(&clk->win, true);

    return clk;
}

void
vlGui_clockSetTime(struct vlGui_clock_t *clk, struct vlGui_clock_time_t *time)
{
    memcpy(&clk->time, time, sizeof(struct vlGui_clock_time_t));
}