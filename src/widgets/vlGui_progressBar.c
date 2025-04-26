/**
 * @file vlGui_progressBar.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-04-22
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
#include <string.h>
#include "vlGui.h"
#include "vlGui_base.h"
#include "widgets/vlGui_progressBar.h"
#include "fonts/vlGui_fonts.h"

#define VLGUI_PROGRESSBAR_LEFT_MARGIN (5)
#define VLGUI_PROGRESSBAR_TOP_MARGIN (3)

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */
/**
 * @brief Callback function for progress bar to draw this window.
 * If want to implement special effect, can modify here.
 *
 * @param win
 * @param arg
 */
static void
vlGui_progressBarDrawCb(vlGui_window_t *win, uint8_t arg)
{
    if (arg == VLGUI_WIN_DRAW_INIT) {
        vlGui_windowClear(win);
    }
    vlGui_progressBarDrawWin(win, (vlGui_progressBarData_t *)win->userData, arg);
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
int
vlGui_progressBarProcessKey(vlGui_window_t *win, uint8_t key)
{
    vlGui_progressBarData_t *data;

    data = (vlGui_progressBarData_t *)&win->userData;

    switch (key) {
    case VLGUI_KEY_ESC:
        return -1;
    case VLGUI_KEY_RIGHT:
        vlGui_progressBarSetValue(data, data->value + 10);
        return 1;
    case VLGUI_KEY_LEFT:
        vlGui_progressBarSetValue(data, data->value - 10);
        return 1;
    default:
        return 0;
    }
}

void
vlGui_progressBarDrawWin(vlGui_window_t *win, vlGui_progressBarData_t *pbarData, uint8_t flag)
{
    uint16_t len;
    char num[5];
    const struct vlGui_font_t *font;
    int16_t numPosX;
    int16_t numPosY;
    int16_t titleHight;
    int16_t barWidth;

    font = vlGui_asc6x8;
    vlGui_setFont(font);

    titleHight = VLGUI_PROGRESSBAR_TOP_MARGIN + font->fontHeight;
    numPosX = win->win_width - VLGUI_PROGRESSBAR_LEFT_MARGIN - (3 * font->fontWidth);
    numPosY = titleHight + VLGUI_STR_CENTER_Y(win->win_height - titleHight, font, 1);
    barWidth = numPosX - 2 * VLGUI_PROGRESSBAR_LEFT_MARGIN - 4;

    if (flag == VLGUI_WIN_DRAW_INIT) {
        /* Draw boundary of window */
        vlGui_drawRectangle(win, 1, 1, win->win_width - 2, win->win_height - 2, VLGUI_COLOR_WHITE);
        /* Draw title if has */
        if (pbarData->title) {
            vlGui_drawString(win, VLGUI_PROGRESSBAR_LEFT_MARGIN, VLGUI_PROGRESSBAR_TOP_MARGIN,
                             pbarData->title, VLGUI_COLOR_WHITE);
        }
        /* Draw boundary of progress bar. Reserve space for percentage value */
        vlGui_drawRectangle(win, VLGUI_PROGRESSBAR_LEFT_MARGIN, numPosY - 2, barWidth + 4,
                            font->fontHeight + 2, VLGUI_COLOR_WHITE);
    } else {
        vlGui_drawBlock(win, numPosX, numPosY, 3 * font->fontWidth, font->fontHeight,
                        VLGUI_COLOR_BLACK);
    }
    /* Draw process bar with active color */
    if (pbarData->value == 100) {
        vlGui_drawBlock(win, VLGUI_PROGRESSBAR_LEFT_MARGIN + 2, numPosY, barWidth,
                        font->fontHeight - 2, VLGUI_COLOR_WHITE);
    } else {
        len = (barWidth * pbarData->value) / 100;
        vlGui_drawBlock(win, VLGUI_PROGRESSBAR_LEFT_MARGIN + 2, numPosY, len, font->fontHeight - 2,
                        VLGUI_COLOR_WHITE);
        vlGui_drawBlock(win, VLGUI_PROGRESSBAR_LEFT_MARGIN + 2 + len, numPosY, barWidth - len,
                        font->fontHeight - 2, VLGUI_COLOR_BLACK);
    }

    /* Draw the value of percentage */
    if (pbarData->value > 99) {
        vlGui_drawString(win, numPosX, numPosY, "100", 1);
    } else {
        snprintf(num, sizeof(num), "%02d%%", pbarData->value);
        vlGui_drawString(win, numPosX, numPosY, num, 1);
    }
}

/**
 * @brief Set the percentage of progress bar. The maximum value is 100.
 *
 * @param pbarData
 * @param value
 */
void
vlGui_progressBarSetValue(vlGui_progressBarData_t *pbarData, uint8_t value)
{
    if (value > 100) {
        pbarData->value = 100;
    } else {
        pbarData->value = value;
    }
}

/**
 * @brief Set the title of progress bar.
 *
 * @param pbarData
 * @param title
 */
void
vlGui_progressBarSetTitle(vlGui_progressBarData_t *pbarData, char *title)
{
    pbarData->title = title;
}

void
vlGui_progressBarInitData(vlGui_window_t *win, vlGui_progressBarData_t *pbarData)
{
    VLGUI_UNUSED(win);

    vlGui_windowBlurEnable(win, true);
    pbarData->value = 50;
    pbarData->title = "Pls set text";
}

/**
 * @brief Create a window for progress bar.
 *
 * @param parent Parent window
 * @param x      Position of x axis
 * @param y      Position of y axis
 * @param width
 * @param height
 * @return vlGui_progressBar_t*
 */
vlGui_progressBar_t *
vlGui_progressBarCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width,
                        uint16_t height)
{
    vlGui_progressBar_t *pbar;
    VLGUI_ASSERT(parent);

    pbar = (vlGui_progressBar_t *)vlGui_windowCreate(parent, x, y, width, height,
                                                     sizeof(vlGui_progressBarData_t));
    if (!pbar) {
        return NULL;
    }

    pbar->win.type = VLGUI_WIN_TYPE_PROGBAR;
    vlGui_progressBarInitData(&pbar->win, &pbar->data);
    vlGui_windowSetDrawCb(&pbar->win, vlGui_progressBarDrawCb);
    vlGui_windowSetKeyCb(&pbar->win, vlGui_progressBarProcessKey);

    return pbar;
}

void
vlGui_progressBarDestroy(vlGui_progressBar_t *pbar)
{
    VLGUI_UNUSED(pbar);
}