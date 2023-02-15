/**
 * @file vlGui_selector.c
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
#include "vlGui.h"
#include "vlGui_base.h"
#include "vlGui_input.h"
#include "vlGui_selector.h"
#include <string.h>

struct vlGui_selectorEntry_t {
    uint8_t index;
    char * str;
    struct vlGui_selectorEntry_t *next;
};

static void
vlGui_selectorScrollUpCb(void *arg)
{
    struct vlGui_selector_t *sel;

    sel = (struct vlGui_selector_t *)arg;
    sel->index = sel->temp_index;
}

static void
vlGui_selectorScrollDownCb(void *arg)
{
    struct vlGui_selector_t *sel;

    sel = (struct vlGui_selector_t *)arg;
    sel->index = sel->temp_index;
}

int 
vlGui_selectorProcessKey(struct vlGui_window_t *win, uint8_t key)
{
    struct vlGui_selector_t *sel;

    sel = (struct vlGui_selector_t *)win;
    if((key == VLGUI_KEY_UP) && (sel->temp_index > 0)) {
        --sel->temp_index;
        vlGui_windowScrollAnimation(win, 0, sel->bigFont->fontHeight + 4, 300, vlGui_selectorScrollUpCb, sel);
    } else if((key == VLGUI_KEY_DOWN) && (sel->temp_index < (sel->num - 1))) {
        ++sel->temp_index;
        vlGui_windowScrollAnimation(win, 0, -sel->bigFont->fontHeight - 4, 300, vlGui_selectorScrollDownCb, sel);
    }

    return 0;
}

static void 
vlGui_drawSelector(struct vlGui_window_t *win, void *arg)
{
    struct vlGui_selector_t *sel;
    struct vlGui_selectorEntry_t *entry;
    const struct vlGui_font_t *font;
    int16_t h, y;
    uint8_t index;
    int16_t ax, ay;

    sel = (struct vlGui_selector_t *)win;

    index = sel->index;
    h = sel->bigFont->fontHeight + 4;

    entry = (struct vlGui_selectorEntry_t *)sel->entry;

    vlGui_windowClear(win);

    y = win->win_height >> 1;
    while(entry) {
        if(entry->index != index) {
            font  = sel->smallFont;
        } else {
            font  = sel->bigFont;
        }

        ax = (win->win_width - (font->fontWidth * strlen(entry->str))) >> 1;
        ay = y - (font->fontHeight >> 1);

        vlGui_setFont(font);
        vlGui_drawString(win, ax, ay, entry->str, 1);
        entry = entry->next;

        y += h;
    }

    y = ((win->win_height - sel->bigFont->fontHeight) >> 1) - 2 - win->y_offset;

    vlGui_drawRectangle(win, 0, y, 
                          win->win_width, sel->bigFont->fontHeight + 4, 1);
}

struct vlGui_selector_t *
vlGui_selectorCreate(struct vlGui_window_t *parent)
{
    struct vlGui_selector_t *sel;
    VLGUI_ASSERT(parent);

    sel = (struct vlGui_selector_t *)vlGui_windowCreate(parent, 0, 0, 
                                parent->win_width, parent->win_height, 
                                sizeof(*sel) - sizeof(*parent));
    if(!sel) {
        return sel;
    }

    sel->win.type = VLGUI_WIN_TYPE_SELECTOR;
    sel->win.pDrawWindow = vlGui_drawSelector;
    sel->win.pProcessKey = vlGui_selectorProcessKey;
    sel->smallFont = &vlGui_font7x10;
    sel->bigFont   = &vlGui_font11x18;
    sel->index = 0;
    sel->temp_index = 0;
    sel->num = 0;
    sel->entry = (void *)0;

    return sel;
}

int
vlGui_selectorAddEntry(struct vlGui_selector_t *sel, char *str)
{
    struct vlGui_selectorEntry_t *entry;
    struct vlGui_selectorEntry_t *last;

    entry = vlGui_malloc(sizeof(*entry));

    if(!entry) {
        return -1;
    }

    entry->str = str;
    entry->index = sel->num;
    entry->next  = NULL;
    ++sel->num;

    // struct vlGui_window_t 
    if (sel->entry) {
        for (last = sel->entry; last->next; last = last->next);
        last->next = entry;
    } else {
        sel->entry  = (void *)entry;
    }

    return 0;
}

void
vlGui_selectorDelete(struct vlGui_selector_t *sel)
{

}