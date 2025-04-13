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
#include "vlGui_fonts.h"

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

/**
 * @brief Callback function for progress bar to process coming key.
 *
 * @param win
 * @param key
 * @return int
 */
int
vlGui_progressBarProcessKey(vlGui_window_t *win, uint8_t key)
{
    VLGUI_UNUSED(win);
    VLGUI_UNUSED(key);

    // struct vlGui_selector_t *sel;

    // sel = (struct vlGui_selector_t *)win;
    // if((key == VLGUI_KEY_UP) && (sel->temp_index > 0)) {
    //     --sel->temp_index;
    //     vlGui_windowScrollAnimation(win, 0, sel->bigFont->fontHeight + 4, 300,
    //     vlGui_selectorScrollUpCb, sel);
    // } else if((key == VLGUI_KEY_DOWN) && (sel->temp_index < (sel->num - 1))) {
    //     ++sel->temp_index;
    //     vlGui_windowScrollAnimation(win, 0, -sel->bigFont->fontHeight - 4, 300,
    //     vlGui_selectorScrollDownCb, sel);
    // }
    return 0;
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
void
vlGui_progressBarDrawWin(vlGui_window_t *win, vlGui_progressBarData_t *pbarData, uint8_t flag)
{
    uint16_t len;
    char num[5];
    char *title;
    const struct vlGui_font_t *font;

    VLGUI_UNUSED(flag);
    font = &vlGui_font6x8;

    vlGui_setFont(font);
    /* Clear window */
    vlGui_windowClear(win);
    /* Draw boundary of window */
    vlGui_drawRectangle(win, 1, 1, win->win_width - 2, win->win_height - 2, 1);

    /* Draw boundary of progress bar. Reserve space for percentage value */
    vlGui_drawRectangle(win, 5, (win->win_height >> 1) - 2, win->win_width - 10 - 25, 10, 1);
    /* Draw process bar with active color */
    len = (win->win_width - 14 - 25) * pbarData->value / 100;
    vlGui_drawBlock(win, 7, (win->win_height >> 1), len, 6, 1);

    vlGui_setFont(&vlGui_font6x8);

    if (pbarData->value > 99) {
        num[0] = 'O';
        num[1] = 'k';
        num[2] = '\0';
    } else {
        snprintf(num, sizeof(num), "%02d", pbarData->value);
    }
    vlGui_drawString(win, win->win_width - 10 - 15, win->win_height >> 1, num, 1);
    /* Draw title if has */
    title = pbarData->title;
    if (title) {
        len = strlen(title) * font->fontWidth;
        vlGui_drawString(win, (win->win_width - len) >> 1, 3, pbarData->title, 1);
    }
    /* Draw the value of percentage */
    snprintf(num, sizeof(num), "%02d%%", pbarData->value);
    vlGui_drawString(win, win->win_width - 10 - 15, win->win_height >> 1, num, 1);
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

    pbarData->value = 0;
    pbarData->title = NULL;
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

    pbar = (vlGui_progressBar_t *)vlGui_windowCreate(
        parent, x, y, width, height, sizeof(*pbar) - sizeof(*parent));
    if (!pbar) {
        return NULL;
    }

    pbar->win.type = VLGUI_WIN_TYPE_PROGBAR;
    vlGui_windowSetDrawCb(&pbar->win, vlGui_progressBarDrawCb);
    vlGui_windowSetKeyCb(&pbar->win, vlGui_progressBarProcessKey);

    return pbar;
}

void
vlGui_progressBarDestroy(vlGui_progressBar_t *pbar)
{
    VLGUI_UNUSED(pbar);
}