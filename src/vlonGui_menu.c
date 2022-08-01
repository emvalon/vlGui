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
#include "vlonGui_fonts.h"
#include "vlonGui_input.h"

#define VLGUI_MENU_ENTRY_PADDING        (3)
#define VLGUI_MENU_ENTRY_MARGIN         (3)

struct vlonGui_menuEntry_t {
    uint8_t level;
    uint8_t show;
    uint8_t folded;
    struct vlonGui_menuEntry_t *next;
    struct vlonGui_menuEntry_t *prev;
    char * str;
};

static int 
vlonGui_menuProcessKey(struct vlonGui_window_t *win, uint8_t key)
{
    uint8_t parentLvl;
    uint8_t nextLvl;
    uint8_t show;
    struct vlonGui_menu_t *menu;
    struct vlonGui_menuEntry_t *sel;
    struct vlonGui_menuEntry_t *next;

    menu = (struct vlonGui_menu_t *)win;
    sel = menu->selEntry;

    switch (key)
    {
    case VLGUI_KEY_UP:
    case VLGUI_KEY_LETF:
        /* Move the previous entry */
        while (1) {
            sel = sel->prev;
            if (!sel) {
                return 0;
            } else if (sel->show) {
                menu->selEntry = sel;
                return 0;
            }
        }
    case VLGUI_KEY_DOWN:
    case VLGUI_KEY_RIGHT:
        /* Move to the next entry */
        while (1) {
            sel = sel->next;
            if (!sel) {
                return 0;
            } else if (sel->show) {
                menu->selEntry = sel;
                return 0;
            }
        }
    case VLGUI_KEY_OK:
        /* Unfold selected entry */
        parentLvl = sel->level;
        next = sel->next;
        if (!next) {
            return 0;
        }
        show = sel->folded;
        sel->folded = !show;

        nextLvl = next->level;
        if (nextLvl <= parentLvl) {
            return 0;
        }
        while (1) {
            next->show = show;
            next = next->next;
            if (!next || (next->level != nextLvl)) {
                return 0;
            }
        }
    default:
        return 0;
    }
}

static void 
vlonGui_drawMenu(struct vlonGui_window_t *win, void *arg)
{
    uint8_t index;
    uint8_t selIndex;
    uint8_t unit;
    struct vlonGui_menu_t *menu;
    struct vlonGui_menuEntry_t *entry;
    struct vlonGui_font_t *font;
    int16_t lineHight;
    int16_t y;
    

    menu = (struct vlonGui_menu_t *)win;

    vlonGui_windowClear(win);

    font = &vlonGui_font7x10;
    lineHight = font->FontHeight + 2;
    vlonGui_setFont(font);

    entry = menu->entry;
    index = 0;

    while (entry) {
        if (entry->show) {
            y = index * lineHight;
            vlonGui_drawString(win, VLGUI_MENU_ENTRY_MARGIN + VLGUI_MENU_ENTRY_PADDING + 
                               (font->FontWidth * entry->level * 2), y + 1, entry->str, 1);
            if (entry == menu->selEntry) {
                vlonGui_drawBlock(win, VLGUI_MENU_ENTRY_MARGIN, y, 
                                  win->win_width - (2 * VLGUI_MENU_ENTRY_PADDING) - 2, 
                                  font->FontHeight, VLGUI_COLOR_CONVERT);
                vlonGui_drawPoint(win, VLGUI_MENU_ENTRY_MARGIN, y, VLGUI_COLOR_BLACK);
                vlonGui_drawPoint(win, VLGUI_MENU_ENTRY_MARGIN, y + font->FontHeight - 1,
                                  VLGUI_COLOR_BLACK);
                selIndex = index;
            }
            ++index;
        }
        entry = entry->next;
    }
    unit = win->win_height / index;
    vlonGui_drawRectangle(win, win->win_width - 2, selIndex * unit, 2, unit, VLGUI_COLOR_WHITE);

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

    menu->entry = NULL;
    menu->selEntry = NULL;
    menu->num = 0;
    menu->win.type = VLGUI_WIN_TYPE_MENU;
    menu->win.pDrawWindow = vlonGui_drawMenu;
    menu->win.pProcessKey = vlonGui_menuProcessKey;

    return menu;
}

int
vlonGui_menuAddEntry(struct vlonGui_menu_t *menu, uint16_t index, uint8_t level, char *name)
{
    struct vlonGui_menuEntry_t *entry;
    struct vlonGui_menuEntry_t *prev;
    struct vlonGui_menuEntry_t *next;

    entry = vlonGui_malloc(sizeof(*entry));

    if(!entry) {
        return -1;
    }

    /* Select the 1st entry by default */
    if (!index || !menu->entry) {
        menu->selEntry = entry;
    }

    /* Only dispaly levle 0 by default */
    if (!level) {
        entry->show = 1;
    } else {
        entry->show = 0;
    }

    entry->folded = 1;
    entry->level = level;
    entry->str = name;
    
    /* Insert into list according to index */
    prev = NULL;
    next = menu->entry;
    while (index && next) {
        prev = next;
        next = next->next;
        --index;
    }
    
    entry->prev = prev;
    if (prev) {
        entry->next = prev->next;
        prev->next = entry;
    } else {
        entry->next = menu->entry;
        menu->entry  = (void *)entry;
    }
    ++menu->num;

    return 0;
}