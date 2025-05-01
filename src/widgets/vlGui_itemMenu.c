/**
 * @file vlGui_itemMenu.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2025-05-02
 *
 * Copyright 2024 - 2025 Weilong Shen (valonshen@foxmail.com)
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
#include <string.h>
#include "vlGui.h"
#include "vlGui_base.h"
#include "widgets/vlGui_itemMenu.h"
#include "fonts/vlGui_fonts.h"

#define VLGUI_ITEMMENU_ENGINE_DUR (350)

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */
/**
 * @brief Callback function for progress bar to draw this window.
 * If want to implement special effect, can modify here.
 *
 * @param win
 * @param arg
 */
static void
vlGui_itemMenuDrawCb(vlGui_window_t *win, uint8_t arg)
{
    if (arg == VLGUI_WIN_DRAW_INIT) {
        vlGui_windowClear(win);
    }
    vlGui_itemMenuDrawWin(win, (vlGui_itemMenuData_t *)win->userData, arg);
}

static int
vlGui_itemMenuKeyCb(vlGui_window_t *win, uint8_t key)
{
    return vlGui_itemMenuProcessKey(win, (vlGui_itemMenuData_t *)win->userData, key);
}

static void
vlGui_itemMenuEngineCb(void *param, int16_t delta)
{
    vlGui_itemMenuData_t *data;
    vlGui_scrollBarData_t *sbData;
    data = (vlGui_itemMenuData_t *)param;

    sbData = (vlGui_scrollBarData_t *)&data->sbData;

    data->clear = 1;
    data->offset += delta;

    vlGui_scrollBarSetPosition(sbData, vlGui_scrollBarGetPosition(sbData) + delta);
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
int
vlGui_itemMenuProcessKey(vlGui_window_t *win, vlGui_itemMenuData_t *data, uint8_t key)
{
    VLGUI_UNUSED(win);
    VLGUI_UNUSED(data);
    vlGui_engine_t *engine;
    int16_t itemHight;

    engine = &data->engine;
    itemHight = data->font->fontHeight;

    switch (key) {
    case VLGUI_KEY_UP:
        vlGui_engineStart(engine, itemHight, VLGUI_ITEMMENU_ENGINE_DUR);
        vlGui_windowSetRefresh(win);
        return 0;
    case VLGUI_KEY_DOWN:
        vlGui_engineStart(engine, -itemHight, VLGUI_ITEMMENU_ENGINE_DUR);
        vlGui_windowSetRefresh(win);
        return 0;
    default:
        return -1;
    }
}

void
vlGui_itemMenuDrawWin(vlGui_window_t *win, vlGui_itemMenuData_t *pMenuData, uint8_t flag)
{
    vlGui_itemMenuEntry_t *entry;
    const struct vlGui_font_t *font;
    int16_t itemHight;
    int16_t y;
    vlGui_itemMenuDrawEntryCb_t entryDrawCb;
    vlGui_windowRectArea_t area;
    int rc;

    VLGUI_UNUSED(flag);

    if (pMenuData->clear) {
        vlGui_windowClear(win);
    }

    if ((flag == VLGUI_WIN_DRAW_INIT) || pMenuData->clear) {
        vlGui_scrollBarDrawWin(win, &pMenuData->sbData, flag);
    }

    font = pMenuData->font;
    itemHight = font->fontHeight;
    entry = pMenuData->entries;
    y = pMenuData->offset;

    vlGui_setFont(font);
    for (; entry; entry = entry->next, y += itemHight) {
        if ((y + itemHight) < 0) {
            continue;
        }
        if (y >= win->win_height) {
            break;
        }
        entryDrawCb = pMenuData->entryDrawCb;
        if (entryDrawCb) {
            area.x = 0;
            area.y = y;
            area.width = win->win_width;
            area.hight = itemHight;
            rc = entryDrawCb(win, &area, entry);
        } else {
            rc = -1;
        }
        if (rc) {
            vlGui_drawString(win, 0, y, entry->text, VLGUI_COLOR_WHITE);
        }
    }
}

void
vlGui_itemMenuInitData(vlGui_window_t *win, vlGui_itemMenuData_t *pMenuData,
                       const struct vlGui_font_t *font, vlGui_engineCurve_t engineCurve)
{
    VLGUI_UNUSED(engineCurve);

    memset(pMenuData, 0, sizeof(vlGui_itemMenuData_t));
    pMenuData->font = font;

    vlGui_engineInit(&pMenuData->engine, engineCurve, vlGui_itemMenuEngineCb, pMenuData);
    vlGui_windowAppendEngine(win, &pMenuData->engine);

    vlGui_scrollBarInitData(win, &pMenuData->sbData, 1);
    vlGui_scrollBarSetVisible(&pMenuData->sbData, win->win_height);
}

void
vlGui_itemMenuAddEntry(vlGui_itemMenuData_t *pMenuData, vlGui_itemMenuEntry_t *entry)
{
    vlGui_itemMenuEntry_t *last;
    vlGui_scrollBarData_t *sbData;
    int16_t itemHight;

    itemHight = pMenuData->font->fontHeight;
    entry->next = NULL;

    last = pMenuData->entries;
    if (!last) {
        pMenuData->entries = entry;
        return;
    }

    while (last->next) {
        last = last->next;
    }
    last->next = entry;

    sbData = (vlGui_scrollBarData_t *)&pMenuData->sbData;
    vlGui_scrollBarSetRange(sbData, vlGui_scrollBarGetRange(sbData) + itemHight);
}

void
vlGui_itemMenuSetEntryDrawCb(vlGui_itemMenuData_t *pMenuData, vlGui_itemMenuDrawEntryCb_t cb)
{
    pMenuData->entryDrawCb = cb;
}

/**
 * @brief Create a window for progress bar.
 *
 * @param parent Parent window
 * @param x      Position of x axis
 * @param y      Position of y axis
 * @param width
 * @param height
 * @return vlGui_itemMenu_t*
 */
vlGui_itemMenu_t *
vlGui_itemMenuCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width, uint16_t height)
{
    vlGui_window_t *win;
    vlGui_itemMenuData_t *menu;

    VLGUI_ASSERT(parent);

    win = vlGui_windowCreate(parent, x, y, width, height, sizeof(vlGui_itemMenuData_t));
    if (!win) {
        return NULL;
    }

    menu = (vlGui_itemMenuData_t *)win->userData;
    vlGui_itemMenuInitData(win, menu, vlGui_asc7x10, vlGui_engineCurveNone);
    vlGui_windowSetDrawCb(win, vlGui_itemMenuDrawCb);
    vlGui_windowSetKeyCb(win, vlGui_itemMenuKeyCb);

    return (vlGui_itemMenu_t *)win;
}

void
vlGui_itemMenuDestroy(vlGui_itemMenu_t *pMenu)
{
    VLGUI_UNUSED(pMenu);
}
