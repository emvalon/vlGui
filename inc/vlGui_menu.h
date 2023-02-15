/**
 * @file vlGui_menu.h
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
#ifndef _VLGUI_MENU_H_
#define _VLGUI_MENU_H_

#include <stdint.h>
#include "vlGui_window.h"
#include "vlGui_input.h"

struct vlGui_menu_t {
    struct vlGui_window_t win;
    const struct vlGui_font_t *font;
    void *entries;
    void *selEntry;
    uint16_t num;
    int16_t selectedRectOffset;
    int16_t menuOffset;
    vlGui_engine_t engine;
};


int vlGui_menuProcessKey(struct vlGui_window_t *win, uint8_t key);

struct vlGui_menu_t * vlGui_menuCreate(struct vlGui_window_t *parent, int16_t x, int16_t y,     
                                           int16_t width, uint16_t height);


int vlGui_menuAddEntry(struct vlGui_menu_t *menu, uint16_t index, uint8_t level, char *name);


void vlGui_menuSetFont(struct vlGui_menu_t *menu, const struct vlGui_font_t *font);


char *vlGui_menuGetSelectedEntry(struct vlGui_menu_t *menu);

#endif // _VLGUI_MENU_H_

