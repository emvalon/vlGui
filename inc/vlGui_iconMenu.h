/**
 * @file vlGui_iconMenu.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-14
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
#ifndef _VLGUI_ICONMENU_H_
#define _VLGUI_ICONMENU_H_

#include <stdint.h>
#include "vlGui.h"
#include "vlGui_window.h"
#include "vlGui_input.h"
#include "vlGui_fonts.h"
#include "vlGui_engine.h"

struct vlGui_iconMenuItem
{
    uint8_t iconWidth;
    uint8_t iconHeight;
    char *name;
    uint8_t *icon;
    struct vlGui_iconMenuItem *next;
    struct vlGui_iconMenuItem *prev;
};
typedef struct vlGui_iconMenuItem vlGui_iconMenuItem_t;

struct vlGui_iconMenu
{
    vlGui_window_t win;
    const struct vlGui_font_t *font;
    vlGui_iconMenuItem_t *items;
    vlGui_engine_t engine;
    vlGui_engine_t strEngine;

    uint8_t iconMaxSize;
    uint8_t selectedIndex;
    uint8_t previousIndex;
    uint8_t numOfItems;
    int16_t offset;
    int16_t strOffset;
};
typedef struct vlGui_iconMenu vlGui_iconMenu_t;


int vlGui_iconMenuProcessKey(vlGui_window_t *win, uint8_t key);

vlGui_iconMenu_t * vlGui_iconMenuCreate(vlGui_window_t *parent, int16_t x,
                int16_t y, int16_t width, uint16_t height, uint8_t iconMAxSize);

void vlGui_iconMenuAddItem(vlGui_iconMenu_t *menu, vlGui_iconMenuItem_t *item);

void vlGui_iconMenuSetFont(vlGui_iconMenu_t *menu, 
                           const struct vlGui_font_t *font);
#endif // _VLGUI_ICONMENU_H_