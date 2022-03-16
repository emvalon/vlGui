/**
 * @file games.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-21
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
#include "games.h"
#include "games_glatSnake.h"
#include "vlGui_selector.h"

struct vlGui_selector_t *sel;

static int 
gameSelectorKeyCb(vlGui_window_t *win, uint8_t key)
{
    if (key == VLGUI_KEY_OK) {
        switch (sel->index) {
        case 0:
            games_gultSnakeCreate(win);
            break;
        default:
            break;
        }
    } else {
        vlGui_selectorProcessKey(win, key);
    }
}

void 
games_createSelector(vlGui_window_t *win) 
{
    sel = vlGui_selectorCreate(win);

    vlGui_selectorAddEntry(sel, "GlutSnake");
    vlGui_selectorAddEntry(sel, "Shen Weilong");
    vlGui_selectorAddEntry(sel, "Valon Shen");
    vlGui_selectorAddEntry(sel, "Ma Suhong");
    vlGui_selectorAddEntry(sel, "Suhon Ma");
    vlGui_selectorAddEntry(sel, "VlonGui"); 

    vlGui_windowSetKeyCb((vlGui_window_t *)sel, gameSelectorKeyCb);
}