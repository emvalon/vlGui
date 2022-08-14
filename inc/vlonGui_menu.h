/**
 * @file vlonGui_menu.h
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
#ifndef _VLONGUI_MENU_H_
#define _VLONGUI_MENU_H_

#include <stdint.h>
#include "vlonGui_window.h"
#include "vlonGui_input.h"

struct vlonGui_menu_t {
    struct vlonGui_window_t win;
    uint16_t num;
    int16_t  yPos;
    struct vlonGui_font_t *font;
    void *entry;
    void *selEntry;
};


int vlonGui_menuProcessKey(struct vlonGui_window_t *win, uint8_t key);

struct vlonGui_menu_t * vlonGui_menuCreate(struct vlonGui_window_t *parent, int16_t x, int16_t y,     
                                           int16_t width, uint16_t height);


int vlonGui_menuAddEntry(struct vlonGui_menu_t *menu, uint16_t index, uint8_t level, char *name);


void vlonGui_menuSetFont(struct vlonGui_menu_t *menu, struct vlonGui_font_t *font);


char *vlonGui_menuGetSelectedEntry(struct vlonGui_menu_t *menu);

#endif // _VLONGUI_MENU_H_

