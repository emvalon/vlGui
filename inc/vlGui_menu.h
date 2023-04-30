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


struct vlGui_menuEntry {
    uint8_t show:1;
    uint8_t checkbox:1;
    uint8_t isChecked:1;
    uint8_t boxAnimation:1;
    uint8_t radioButton:1;

    int8_t level;
    uint8_t folded;
    uint8_t childrenNum;
    struct vlGui_menuEntry *next;
    struct vlGui_menuEntry *prev;
    char * str;
};
typedef struct vlGui_menuEntry vlGui_menuEntry_t;

struct vlGui_menu {
    vlGui_window_t win;
    vlGui_engine_t mainEngine;
    vlGui_engine_t auxEngine;
    const struct vlGui_font_t *font;
    vlGui_menuEntry_t *entries;
    vlGui_menuEntry_t *selEntry;
    uint16_t num;
    int16_t selectedRectOffset;
    int16_t menuOffset;
    union
    {
        int16_t childOffset;
        int16_t boxRatio;
    };
    
    int16_t selectedRectRetract;
    uint16_t flags;
    uint8_t lineHight;
};
typedef struct vlGui_menu vlGui_menu_t;


int vlGui_menuProcessKey(vlGui_window_t *win, uint8_t key);

vlGui_menu_t * vlGui_menuCreate(vlGui_window_t *parent, int16_t x, int16_t y,     
                                           int16_t width, uint16_t height);

void vlGui_menuAddEntry(vlGui_menu_t *menu, vlGui_menuEntry_t *parent,
                        vlGui_menuEntry_t *new, char *text);

void vlGui_menuSetEntryAsCheckbox(vlGui_menuEntry_t *entry);

void vlGui_menuSetEntryAsRadioButton(vlGui_menuEntry_t *entry);

void vlGui_menuSetFont(vlGui_menu_t *menu, const struct vlGui_font_t *font);


char *vlGui_menuGetSelectedEntry(vlGui_menu_t *menu);

#endif // _VLGUI_MENU_H_

