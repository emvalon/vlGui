/**
 * @file vlonGui_selector.c
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
#include "vlonGui.h"
#include "vlonGui_base.h"
#include "vlonGui_input.h"
#include "vlonGui_selector.h"
#include <string.h>

struct vlonGui_selectorEntry_t {
    uint8_t index;
    char * str;
    struct vlonGui_selectorEntry_t *next;
};

static void
vlonGui_selectorScrollUpCb(void *arg)
{
    struct vlonGui_selector_t *sel;

    sel = (struct vlonGui_selector_t *)arg;
    sel->index = sel->temp_index;
}

static void
vlonGui_selectorScrollDownCb(void *arg)
{
    struct vlonGui_selector_t *sel;

    sel = (struct vlonGui_selector_t *)arg;
    sel->index = sel->temp_index;
}

int 
vlonGui_selectorProcessKey(struct vlonGui_window_t *win, uint8_t key)
{
    struct vlonGui_selector_t *sel;

    sel = (struct vlonGui_selector_t *)win;
    if((key == VLGUI_KEY_UP) && (sel->temp_index > 0)) {
        --sel->temp_index;
        vlonGui_windowScrollAnimation(win, 0, sel->bigFont->FontHeight + 4, 300, vlonGui_selectorScrollUpCb, sel);
    } else if((key == VLGUI_KEY_DOWN) && (sel->temp_index < (sel->num - 1))) {
        ++sel->temp_index;
        vlonGui_windowScrollAnimation(win, 0, -sel->bigFont->FontHeight - 4, 300, vlonGui_selectorScrollDownCb, sel);
    }
}

static void 
vlonGui_drawSelector(struct vlonGui_window_t *win, void *arg)
{
    struct vlonGui_selector_t *sel;
    struct vlonGui_selectorEntry_t *entry;
    struct vlonGui_font_t *font;
    int16_t h, y;
    uint8_t index;
    int16_t ax, ay;

    sel = (struct vlonGui_selector_t *)win;

    index = sel->index;
    h = sel->bigFont->FontHeight + 4;

    entry = (struct vlonGui_selectorEntry_t *)sel->entry;

    vlonGui_windowClear(win);

    y = win->win_height >> 1;
    while(entry) {
        if(entry->index != index) {
            font  = sel->smallFont;
        } else {
            font  = sel->bigFont;
        }

        ax = (win->win_width - (font->FontWidth * strlen(entry->str))) >> 1;
        ay = y - (font->FontHeight >> 1);

        vlonGui_setFont(font);
        vlonGui_drawString(win, ax, ay, entry->str, 1);
        entry = entry->next;

        y += h;
    }

    y = ((win->win_height - sel->bigFont->FontHeight) >> 1) - 2 - win->y_offset;

    vlonGui_drawRectangle(win, 0, y, 
                          win->win_width, sel->bigFont->FontHeight + 4, 1);
}

struct vlonGui_selector_t *
vlonGui_selectorCreate(struct vlonGui_window_t *parent)
{
    uint16_t w,h;
    struct vlonGui_selector_t *sel;
    VLGUI_ASSERT(parent);

    sel = (struct vlonGui_selector_t *)vlonGui_windowCreate(parent, 0, 0, 
                                parent->win_width, parent->win_height, 
                                sizeof(*sel) - sizeof(*parent));
    if(!sel) {
        return sel;
    }

    sel->win.type = VLGUI_WIN_TYPE_SELECTOR;
    sel->win.pDrawWindow = vlonGui_drawSelector;
    sel->win.pProcessKey = vlonGui_selectorProcessKey;
    sel->smallFont = &vlonGui_font7x10;
    sel->bigFont   = &vlonGui_font11x18;
    sel->index = 0;
    sel->temp_index = 0;
    sel->num = 0;
    sel->entry = (void *)0;

    return sel;
}

int
vlonGui_selectorAddEntry(struct vlonGui_selector_t *sel, char *str)
{
    struct vlonGui_selectorEntry_t *entry;
    struct vlonGui_selectorEntry_t *last;
    struct vlonGui_window_t *win;

    entry = vlonGui_malloc(sizeof(*entry));

    if(!entry) {
        return -1;
    }

    entry->str = str;
    entry->index = sel->num;
    entry->next  = NULL;
    ++sel->num;

    // struct vlonGui_window_t 
    if (sel->entry) {
        for (last = sel->entry; last->next; last = last->next);
        last->next = entry;
    } else {
        sel->entry  = (void *)entry;
    }
}

void
vlonGui_selectorDelete(struct vlonGui_selector_t *sel)
{

}