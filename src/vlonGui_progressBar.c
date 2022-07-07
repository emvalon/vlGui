/**
 * @file vlonGui_progressBar.c
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
#include "vlonGui.h"
#include "vlonGui_base.h"
#include "vlonGui_progressBar.h"
#include "vlonGui_fonts.h"

/**
 * @brief Callback function for progress bar to draw this window.
 * If want to implement special effect, can modify here. 
 * 
 * @param win 
 * @param arg 
 */
static void 
vlonGui_drawProgressBar(struct vlonGui_window_t *win, void *arg)
{
    uint16_t len;
    char num[4];
    char *title;
    struct vlonGui_font_t *font;
    struct vlonGui_progressBar_t *pbar;

    pbar = (struct vlonGui_progressBar_t *)win;
    font = &vlonGui_font6x8;

    vlonGui_setFont(font);
    /* Clear window */
    vlonGui_windowClear(win);
    /* Draw boundary of window */
    vlonGui_drawRectangle(win, 1, 1, win->win_width - 2, win->win_height - 2, 1);

    /* Draw boundary of progress bar. Reserve space for percentage value */
    vlonGui_drawRectangle(win, 5, (win->win_height >> 1) - 2, win->win_width - 10 - 25, 10, 1);
    /* Draw process bar with active color */
    len = (win->win_width - 14 - 25) * pbar->value / 100;
    vlonGui_drawBlock(win, 7, (win->win_height >> 1), len, 6, 1);

    vlonGui_setFont(&vlonGui_font6x8);

    if (pbar->value > 99) {
        num[0] = 'O';
        num[1] = 'k';
        num[2] = '\0';
    } else {
        snprintf(num, sizeof(num), "%02d", pbar->value);
    }
    vlonGui_drawString(win, win->win_width - 10 - 15, win->win_height >> 1, num, 1);
    /* Draw title if has */
    title = pbar->title;
    if (title) {
        len = strlen(title) * font->FontWidth;
        vlonGui_drawString(win, (win->win_width - len) >> 1, 3, pbar->title, 1);
    }
    /* Draw the value of percentage */
    snprintf(num, sizeof(num), "%02d%%", pbar->value);
    vlonGui_drawString(win, win->win_width - 10 - 15, win->win_height >> 1, num, 1);
}

/**
 * @brief Callback function for progress bar to process coming key.
 * 
 * @param win 
 * @param key 
 * @return int 
 */
int 
vlonGui_progressBarProcessKey(struct vlonGui_window_t *win, uint8_t key)
{
    // struct vlonGui_selector_t *sel;

    // sel = (struct vlonGui_selector_t *)win;
    // if((key == VLGUI_KEY_UP) && (sel->temp_index > 0)) {
    //     --sel->temp_index;
    //     vlonGui_windowScrollAnimation(win, 0, sel->bigFont->FontHeight + 4, 300, vlonGui_selectorScrollUpCb, sel);
    // } else if((key == VLGUI_KEY_DOWN) && (sel->temp_index < (sel->num - 1))) {
    //     ++sel->temp_index;
    //     vlonGui_windowScrollAnimation(win, 0, -sel->bigFont->FontHeight - 4, 300, vlonGui_selectorScrollDownCb, sel);
    // }
    return 0;
}

/**
 * @brief Set the percentage of progress bar. The maximum value is 100.
 * 
 * @param pbar 
 * @param value 
 */
void
vlonGui_progressBarSetValue(struct vlonGui_progressBar_t * pbar, uint8_t value)
{
    if (value > 100) {
        pbar->value = 100;
    } else {
        pbar->value = value;
    }
}

/**
 * @brief Set the title of progress bar.
 * 
 * @param pbar 
 * @param title 
 */
void
vlonGui_progressBarSetTitle(struct vlonGui_progressBar_t * pbar, char *title)
{
    pbar->title = title;
}

/**
 * @brief Create a window for progress bar.
 * 
 * @param parent Parent window
 * @param x      Position of x axis
 * @param y      Position of y axis
 * @param width  
 * @param height 
 * @return struct vlonGui_progressBar_t* 
 */
struct vlonGui_progressBar_t * 
vlonGui_progressBarCreate(struct vlonGui_window_t *parent, int16_t x, int16_t y, 
                          int16_t width, uint16_t height)
{
    struct vlonGui_progressBar_t *pbar;
    VLGUI_ASSERT(parent);

    pbar = (struct vlonGui_progressBar_t *)vlonGui_windowCreate(parent, x, y, 
                                width, height, sizeof(*pbar) - sizeof(*parent));
    if(!pbar) {
        return NULL;
    }

    pbar->win.type = VLGUI_WIN_TYPE_PROGBAR;
    pbar->win.pDrawWindow = vlonGui_drawProgressBar;
    pbar->win.pProcessKey = vlonGui_progressBarProcessKey;
    pbar->value = 0;
    pbar->title = NULL;

    return pbar;
}

void 
vlonGui_progressBarDestroy(struct vlonGui_progressBar_t *pbar)
{
    vlonGui_windowDelete((struct vlonGui_window_t *)pbar);
}