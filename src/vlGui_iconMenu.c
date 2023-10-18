/**
 * @file vlGui_iconMenu.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-15
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
#include <stdio.h>
#include "vlGui_iconMenu.h"

#define VLGUI_ICONMENU_BOTTOM_MARGIN    (2)
#define VLGUI_ICONMENU_MOVE_DUR         (700)
#define VLGUI_ICONMENU_SEL_WID          (2)
#define VLGUI_ICONMENU_SEL_LEN          (5)

static vlGui_iconMenuItem_t *
vlGui_drawIcons(vlGui_iconMenu_t *menu, int16_t offset, uint16_t y)
{
    int16_t x;
    uint8_t idx;
    vlGui_iconMenuItem_t *item;
    vlGui_iconMenuItem_t *selItem;

    selItem = NULL;
    item = menu->items;
    idx = 0;
    x = offset + (menu->iconMaxSize >> 1);
    y += menu->iconMaxSize >> 1;

    while (item) {
        vlGui_drawBitmap(&menu->win, x - (item->iconWidth >> 1), 
                         y - (item->iconHeight >> 1), item->iconWidth, 
                         item->iconHeight, item->icon);
        x += menu->iconMaxSize + ( 2 * VLGUI_ICONMENU_SEL_WID);
        if (idx == menu->selectedIndex) {
            selItem = item;
        }
        
        ++idx;
        item = item->next;
    }
    VLGUI_ASSERT(selItem);

    return selItem;
}

static void 
vlGui_drawIconMenu(vlGui_window_t *win, uint8_t flag)
{
    int16_t x,y;
    int16_t strPos;;
    vlGui_iconMenu_t *menu;
    const struct vlGui_font_t *font;
    vlGui_iconMenuItem_t *selItem;
    
    VLGUI_UNUSED(flag);
    menu = (vlGui_iconMenu_t *)win;
    vlGui_windowClear(win);

    x = ((win->win_width - menu->iconMaxSize) >> 1) - 1 - VLGUI_ICONMENU_SEL_WID;
    y = 0;
    vlGui_drawLine(win, x, y, x + VLGUI_ICONMENU_SEL_LEN, y, 
                   VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    vlGui_drawLine(win, x, y, x, y + VLGUI_ICONMENU_SEL_LEN, 
                   VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    y = menu->iconMaxSize + VLGUI_ICONMENU_SEL_WID + 2;
    // vlGui_drawLine(win, x, y, x + VLGUI_ICONMENU_SEL_LEN, y, 
    //                VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    // vlGui_drawLine(win, x, y - VLGUI_ICONMENU_SEL_LEN + VLGUI_ICONMENU_SEL_WID, 
    //                x, y, VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    x += menu->iconMaxSize + 2 + VLGUI_ICONMENU_SEL_WID;
    y = 0;
    // vlGui_drawLine(win, x, y, x - VLGUI_ICONMENU_SEL_LEN + VLGUI_ICONMENU_SEL_WID, 
    //                y, VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    // vlGui_drawLine(win, x, y, x, y + VLGUI_ICONMENU_SEL_LEN, 
    //                VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    y = menu->iconMaxSize + VLGUI_ICONMENU_SEL_WID + 2;
    vlGui_drawLine(win, x, y, x - VLGUI_ICONMENU_SEL_LEN + VLGUI_ICONMENU_SEL_WID,
                   y, VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    vlGui_drawLine(win, x, y + 1, x, y - VLGUI_ICONMENU_SEL_LEN + 
                   VLGUI_ICONMENU_SEL_WID, VLGUI_ICONMENU_SEL_WID, 
                   VLGUI_COLOR_WHITE);

    selItem = vlGui_drawIcons(menu, menu->offset, VLGUI_ICONMENU_SEL_WID + 1);

    /* Draw the icon name */
    font = menu->font;
    vlGui_setFont(font);
    strPos = font->fontHeight + VLGUI_ICONMENU_BOTTOM_MARGIN;
    y = win->win_height - strPos;
    
    vlGui_drawBlock(win, 0, y, 8 - (6.0 * menu->strOffset / strPos), 
                    font->fontHeight, VLGUI_COLOR_WHITE);
    /* If the icon is moving in the first step, display the name of the 
     * previous icon.
     */
    if (menu->previousIndex != 0xff) {
        if (menu->previousIndex > menu->selectedIndex) {
            selItem = selItem->next;
        } else {
            selItem = selItem->prev;
        }
    }
    vlGui_drawString(win, VLGUI_STR_CENTER_X(win->win_width, font, 4), 
                     menu->strOffset + y, selItem->name, VLGUI_COLOR_WHITE);
}

int
vlGui_iconMenuProcessKey(vlGui_window_t *win, uint8_t key)
{
    int rc;
    vlGui_iconMenu_t *menu;
    int16_t des;

    rc = 0;
    menu = (vlGui_iconMenu_t *)win;
    
    switch (key)
    {
    case VLGUI_KEY_RIGHT:
        if (menu->selectedIndex >= (menu->numOfItems - 1)) {
            break;
        }
        menu->previousIndex = menu->selectedIndex;
        menu->strOffset = 0;
        ++menu->selectedIndex;
        des = -menu->iconMaxSize - (2 * VLGUI_ICONMENU_SEL_WID);
        vlGui_engineStart(&menu->engine, des, VLGUI_ICONMENU_MOVE_DUR);
        des = menu->font->fontHeight + VLGUI_ICONMENU_BOTTOM_MARGIN;
        vlGui_engineStart(&menu->strEngine, des, VLGUI_ICONMENU_MOVE_DUR >> 1);
        vlGui_windowSetRefresh(win);
        break;
    case VLGUI_KEY_LEFT:
        if (menu->selectedIndex == 0) {
            break;
        }
        menu->previousIndex = menu->selectedIndex;
        menu->strOffset = 0;
        --menu->selectedIndex;
        des = menu->iconMaxSize + (2 * VLGUI_ICONMENU_SEL_WID);
        vlGui_engineStart(&menu->engine, des, VLGUI_ICONMENU_MOVE_DUR);
        des = menu->font->fontHeight + VLGUI_ICONMENU_BOTTOM_MARGIN;
        vlGui_engineStart(&menu->strEngine, des, VLGUI_ICONMENU_MOVE_DUR >> 1);
        vlGui_windowSetRefresh(win);
        break;
    case VLGUI_KEY_ESC:
        rc = -1;
        break;
    default:
        break;
    }

    return rc;
}

static void
vlGui_iconMenuEngineCb(void *param, int16_t delta)
{
    vlGui_iconMenu_t *menu;

    menu = (vlGui_iconMenu_t *)param;
    menu->offset += delta;
    vlGui_windowSetRefresh(&menu->win);
}

static void
vlGui_iconMenuStrEngineCb(void *param, int16_t delta)
{
    vlGui_iconMenu_t *menu;

    menu = (vlGui_iconMenu_t *)param;
    menu->strOffset += delta;
    vlGui_windowSetRefresh(&menu->win);
    if (!vlGui_engineIsRunning(&menu->strEngine) && menu->strOffset) {
        menu->previousIndex = 0xff;
        vlGui_engineStart(&menu->strEngine, -menu->strOffset, 
                          VLGUI_ICONMENU_MOVE_DUR >> 1);
    }
}

vlGui_iconMenu_t * 
vlGui_iconMenuCreate(vlGui_window_t *parent, int16_t x, int16_t y,
                     int16_t width, uint16_t height, uint8_t iconMAxSize)
{
    vlGui_iconMenu_t *menu;

    VLGUI_ASSERT(parent);

    menu = (vlGui_iconMenu_t *)vlGui_windowCreate(parent, x, y, width, height, 
                                            sizeof(*menu) - sizeof(*parent));
    if(!menu) {
        return menu;
    }

    menu->font = vlGui_asc11x18;
    menu->items = NULL;
    menu->iconMaxSize = iconMAxSize;
    menu->selectedIndex = 0;
    menu->previousIndex = 0xff;
    menu->numOfItems = 0;
    menu->win.engines = &menu->engine;
    menu->offset = (width - iconMAxSize) >> 1;
    menu->strOffset = 0;
    
    vlGui_engineInit(&menu->engine, vlGui_engineCurveOutQuint, 
                     vlGui_iconMenuEngineCb, menu);
    vlGui_engineInit(&menu->strEngine, vlGui_engineCurveOutQuint, 
                     vlGui_iconMenuStrEngineCb, menu);
    vlGui_engineAppend(&menu->engine, &menu->strEngine);

    vlGui_windowSetKeyCb(&menu->win, vlGui_iconMenuProcessKey);
    vlGui_windowSetDrawCb(&menu->win, vlGui_drawIconMenu);
    vlGui_windowSetRefresh(&menu->win);

    return menu;
}

void
vlGui_iconMenuAddItem(vlGui_iconMenu_t *menu, vlGui_iconMenuItem_t *item)
{
    vlGui_iconMenuItem_t *last;

    VLGUI_ASSERT(item);

    last = menu->items;
    if (last) {
        while (last->next) {
            last = last->next;
        }
        last->next = item;
        item->prev = last;
    } else {
        menu->items = item;
        item->prev = NULL;
    }
    item->next = NULL;
    ++menu->numOfItems;
}

void
vlGui_iconMenuSetFont(vlGui_iconMenu_t *menu, const struct vlGui_font_t *font)
{
    menu->font = font;
}