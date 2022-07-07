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
#include "vlonGui_selector.h"

struct vlonGui_selector_t *sel;

static int 
gameSelectorKeyCb(struct vlonGui_window_t *win, uint8_t key)
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
        vlonGui_selectorProcessKey(win, key);
    }
}

void 
games_createSelector(struct vlonGui_window_t *win) 
{
    sel = vlonGui_selectorCreate(win);

    vlonGui_selectorAddEntry(sel, "GlutSnake");
    vlonGui_selectorAddEntry(sel, "Shen Weilong");
    vlonGui_selectorAddEntry(sel, "Valon Shen");
    vlonGui_selectorAddEntry(sel, "Ma Suhong");
    vlonGui_selectorAddEntry(sel, "Suhon Ma");
    vlonGui_selectorAddEntry(sel, "VlonGui"); 

    vlonGui_windowSetKeyCb((struct vlonGui_window_t *)sel, gameSelectorKeyCb);
}