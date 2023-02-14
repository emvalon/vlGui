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
#include "stdio.h"
#include "vlonGui_menu.h"
#include "vlonGui_base.h"
#include "vlonGui_fonts.h"
#include "vlonGui_input.h"

#define VLGUI_MENU_ENTRY_PADDING        (1)
#define VLGUI_MENU_ENTRY_MARGIN         (8)
#define VLGUI_MENU_LINE_SPACING         (1)

struct vlonGui_menuEntry_t {
    uint8_t level;
    uint8_t show;
    uint8_t folded;
    struct vlonGui_menuEntry_t *next;
    struct vlonGui_menuEntry_t *prev;
    char * str;
};

int 
vlonGui_menuProcessKey(struct vlonGui_window_t *win, uint8_t key)
{
    uint8_t refresh;
    uint8_t parentLvl;
    uint8_t nextLvl;
    uint8_t show;
    uint8_t lineHeigh;
    struct vlonGui_menu_t *menu;
    struct vlonGui_menuEntry_t *sel;
    struct vlonGui_menuEntry_t *next;

    menu = (struct vlonGui_menu_t *)win;
    sel = menu->selEntry;
    refresh = 0;

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
                lineHeigh = menu->font->fontHeight + (2 * VLGUI_MENU_LINE_SPACING);
                refresh = 1;
                vlonGui_engineStart(&menu->engine, -lineHeigh, 200);
                break;
            }
        }
        break;
    case VLGUI_KEY_DOWN:
    case VLGUI_KEY_RIGHT:
        /* Move to the next entry */
        while (1) {
            sel = sel->next;
            if (!sel) {
                return 0;
            } else if (sel->show) {
                menu->selEntry = sel;
                lineHeigh = menu->font->fontHeight + (2 * VLGUI_MENU_LINE_SPACING);
                refresh = 1;
                vlonGui_engineStart(&menu->engine, lineHeigh, 200);
                break;
            }
        }
        break;
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
                break;
            }
        }
        refresh = 1;
        break;
    default:
        return 0;
    }

    /* If need refresh, set flag */
    if (refresh) {
        vlonGui_windowSetRefresh(win);
    }
    return 0;
}

static void 
vlonGui_drawMenu(struct vlonGui_window_t *win, void *arg)
{
    int16_t y;
    int16_t lineHight;
    int16_t rectOffset;
    uint8_t selIndex;
    uint8_t index;
    uint8_t unit;
    struct vlonGui_menu_t *menu;
    struct vlonGui_menuEntry_t *entry;
    const struct vlonGui_font_t *font;
    

    index = 0;
    menu = (struct vlonGui_menu_t *)win;

    vlonGui_windowClear(win);
    /* Set the text font */
    font = menu->font;
    vlonGui_setFont(font);
    /* Calculate the hight for each line */
    lineHight = font->fontHeight + (2 * VLGUI_MENU_LINE_SPACING);
    rectOffset = menu->selectedRectOffset;

    entry = menu->entries;

    y = menu->menuOffset - lineHight;
    for (entry = menu->entries; entry; entry = entry->next) {
        if (!entry->show) {
            continue;
        }
        ++index;
        y += lineHight;
        /* Check if this entry is in the screen */
        if ((y + lineHight) <= 0) {
            continue;
        } else if (y >= win->win_height) {
            break;
        }
        /* Draw the entry's text */
        vlonGui_drawString(win, VLGUI_MENU_ENTRY_MARGIN + 
                            VLGUI_MENU_ENTRY_PADDING + 
                            (3 * entry->level * 2), 
                            y + VLGUI_MENU_LINE_SPACING, entry->str, 
                            VLGUI_COLOR_WHITE);
        /* Draw the selected entry with opposite color */
        if (entry == menu->selEntry) {
            selIndex = index;
            vlonGui_drawBlock(win, VLGUI_MENU_ENTRY_MARGIN, 0 + rectOffset, 
                              win->win_width - (2 * VLGUI_MENU_ENTRY_MARGIN) - 2,
                              font->fontHeight, VLGUI_COLOR_CONVERT);
            vlonGui_drawPoint(win, VLGUI_MENU_ENTRY_MARGIN, 0 + rectOffset, 
                              VLGUI_COLOR_BLACK);
            vlonGui_drawPoint(win, VLGUI_MENU_ENTRY_MARGIN, 0 + rectOffset +
                              font->fontHeight - 1, VLGUI_COLOR_BLACK);

            vlonGui_drawPoint(win, win->win_width - VLGUI_MENU_ENTRY_MARGIN - 3,
                              0 + rectOffset, VLGUI_COLOR_BLACK);
            vlonGui_drawPoint(win, win->win_width - VLGUI_MENU_ENTRY_MARGIN - 3,
                              0 + rectOffset + font->fontHeight - 1, 
                              VLGUI_COLOR_BLACK);
        }
    }

    // vlonGui_drawLine(win, 11, 8,
    //                  11, 36,
    //                  1, VLGUI_COLOR_WHITE);

    unit = win->win_height / index;
    vlonGui_drawLine(win, win->win_width - 2, 0, 
                     win->win_width - 2, win->win_height,
                     1, VLGUI_COLOR_WHITE);
    vlonGui_drawRectangle(win, win->win_width - 3, (selIndex - 1) * unit, 
                          3, unit, VLGUI_COLOR_WHITE);

}

static void
vlonGui_menuEngineCb(void *param, int16_t delta)
{

    struct vlonGui_menu_t *menu;
    struct vlonGui_window_t *win;
    int16_t lineHight;
    int16_t rectBottom;
    int16_t offset;

    menu = (struct vlonGui_menu_t *)param;

    offset = menu->selectedRectOffset + delta;
    if (offset < 0) {
        menu->menuOffset -= offset;
        menu->selectedRectOffset = 0;
    } else {
        win = &menu->win;
        lineHight = menu->font->fontHeight + (2 * VLGUI_MENU_LINE_SPACING);
        rectBottom = win->win_height - lineHight;
        
        if (offset > rectBottom) {
            menu->menuOffset -= (offset - rectBottom);
            menu->selectedRectOffset = rectBottom;
        } else {
            menu->selectedRectOffset = offset;
        }
    }
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

    menu->entries = NULL;
    menu->selEntry = NULL;
    menu->num = 0;
    menu->font = &vlonGui_font7x10;
    menu->win.type = VLGUI_WIN_TYPE_MENU;
    menu->win.pDrawWindow = vlonGui_drawMenu;
    menu->win.pProcessKey = vlonGui_menuProcessKey;
    menu->menuOffset = 0;
    menu->selectedRectOffset = 0;

    vlonGui_engineInit(&menu->engine, vlonGui_menuEngineCb, menu, 0);
    menu->win.engines = &menu->engine;

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
    if (!index || !menu->entries) {
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
    next = menu->entries;
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
        entry->next = menu->entries;
        menu->entries  = (void *)entry;
    }
    ++menu->num;

    return 0;
}

void
vlonGui_menuSetFont(struct vlonGui_menu_t *menu, const struct vlonGui_font_t *font)
{
    menu->font = font;
    /* Font is changed. Reset menu to initial state. */
    menu->selEntry = menu->entries;
    menu->win.y_offset = 0;
}

char *
vlonGui_menuGetSelectedEntry(struct vlonGui_menu_t *menu)
{
    struct vlonGui_menuEntry_t *sel;

    sel = (struct vlonGui_menuEntry_t *)menu->selEntry;
    if (sel) {
        return sel->str;
    }

    return NULL;
}