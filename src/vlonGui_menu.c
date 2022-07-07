/**
 * @file vlonGui_menu.c
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
#include "vlonGui.h"
#include "vlonGui_menu.h"
#include "vlonGui_base.h"


static int 
vlonGui_menuProcessKey(struct vlonGui_window_t *win, uint8_t key)
{
    struct vlonGui_menu_t *menu;

    menu = (struct vlonGui_menu_t *)win;
}

static void 
vlonGui_drawMenu(struct vlonGui_window_t *win, void *arg)
{
    struct vlonGui_menu_t *menu;
    int16_t w,h, sx, sy;

    menu = (struct vlonGui_menu_t *)win;

    w = win->win_width;
    h = win->win_height;

}

struct vlonGui_menu_t * 
vlonGui_menuCreate(struct vlonGui_window_t *parent, int16_t x, int16_t y,     
                     int16_t width, uint16_t height)
{
    struct vlonGui_menu_t *menu;
    VLGUI_ASSERT(parent);

    menu = (struct vlonGui_menu_t *)vlonGui_windowCreate(parent, x, y,
                            width, height, sizeof(*menu) - sizeof(*parent));
    if(!menu) {
        return menu;
    }

    menu->win.type = VLGUI_WIN_TYPE_BUTTON;
    menu->win.pDrawWindow = vlonGui_drawMenu;
    menu->win.pProcessKey = vlonGui_menuProcessKey;

    return menu;
}