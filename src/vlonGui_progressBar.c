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
#include "vlonGui.h"
#include "vlonGui_base.h"
#include "vlonGui_progressBar.h"


static void 
vlonGui_drawProgressBar(struct vlonGui_window_t *win, void *arg)
{
    int16_t x, y;
    uint16_t len;
    char num[3];
    struct vlonGui_progressBar_t *pbar;

    pbar = (struct vlonGui_progressBar_t *)win;

    vlonGui_windowClear(win);

    vlonGui_drawRectangle(win, 1, 1, win->win_width - 2, win->win_height - 2, 1);


    vlonGui_drawBlock(win, 4, (win->win_height >> 1) - 1, win->win_width - 9, 10, 0);
    vlonGui_drawRectangle(win, 5, (win->win_height >> 1) - 2, win->win_width - 10 - 25, 10, 1);

    len = (win->win_width - 14 - 25) * pbar->value / 100;
    vlonGui_drawBlock(win, 7, (win->win_height >> 1), len, 6, 1);

    vlonGui_setFont(&vlonGui_font6x8);

    snprintf(num, sizeof(num), "%02d", pbar->value);
    vlonGui_drawString(win, win->win_width - 10 - 15, win->win_height >> 1, num, 1);
    ++pbar->value; 
    if (pbar->value == 100) {
        pbar->value = 0;
    }
}

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
}

void
vlonGui_progressBarSetValue(struct vlonGui_progressBar_t * pbar, uint8_t value)
{
    pbar->value = value;
}

struct vlonGui_progressBar_t * 
vlonGui_progressBarCreate(struct vlonGui_window_t *parent, int16_t x, int16_t y, 
                          int16_t width, uint16_t height)
{
    uint16_t w,h;
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

    return pbar;
}