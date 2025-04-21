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
#include <string.h>
#include "widgets/vlGui_iconMenu.h"

#define VLGUI_ICONMENU_TOP_MARGIN (4)
#define VLGUI_ICONMENU_BOTTOM_MARGIN (1)
#define VLGUI_ICONMENU_TEXT_MARGIN (2)
#define VLGUI_ICONMENU_MOVE_DUR (700)
#define VLGUI_ICONMENU_SEL_WID (2)
#define VLGUI_ICONMENU_SEL_LEN (5)
#define VLGUI_ICONMENU_ICON_MARGIN (2)

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */

/**
 * @brief Draw all icons and return the currently selected item.
 */
static vlGui_iconMenuItem_t *
vlGui_drawIcons(vlGui_window_t *win, vlGui_iconMenuData_t *data, uint16_t y)
{
    int16_t x;
    int16_t iconX;
    uint8_t idx = 0;
    uint8_t halfWidth;
    vlGui_iconMenuItem_t *item = data->items;
    vlGui_iconMenuItem_t *selItem = NULL;

    x = data->offset + (data->iconMaxSize >> 1);
    y += data->iconMaxSize >> 1;

    while (item) {
        halfWidth = item->iconWidth >> 1;
        iconX = x - halfWidth;

        if (iconX >= win->win_width) {
            if (selItem) {
                break;
            }
        } else if ((x + halfWidth) >= 0) {
            vlGui_drawBitmap(win, iconX, y - (item->iconHeight >> 1), item->iconWidth,
                             item->iconHeight, item->icon);
        }

        if (idx == data->selectedIndex) {
            selItem = item;
        }

        x += data->iconMaxSize + (2 * VLGUI_ICONMENU_ICON_MARGIN);
        item = item->next;
        ++idx;
    }

    VLGUI_ASSERT(selItem);
    return selItem;
}

/**
 * @brief Draw the icon menu window and highlight the selected icon.
 */
static void
vlGui_iconMenuDrawCb(vlGui_window_t *win, uint8_t flag)
{
    if (flag == VLGUI_WIN_DRAW_INIT) {
        vlGui_windowClear(win);
    }
    vlGui_iconMenuDrawWin(win, (vlGui_iconMenuData_t *)&win->userData, flag);
}

int
vlGui_iconMenuProcessKeyCb(vlGui_window_t *win, uint8_t key)
{
    return vlGui_iconMenuProcessKey(win, (vlGui_iconMenuData_t *)&win->userData, key);
}

/**
 * @brief Callback for main icon menu animation.
 */
static void
vlGui_iconMenuEngineCb(void *param, int16_t delta)
{
    int16_t offset;
    vlGui_iconMenuData_t *data = (vlGui_iconMenuData_t *)param;
    vlGui_window_t *win = data->win;
    vlGui_engine_t *engine;

    engine = &data->engine;
    offset = data->lastOffset + delta;
    data->offset = offset;

    if (!vlGui_engineIsRunning(engine)) {
        data->lastOffset = data->offset;
    }

#if VLGUI_ICONMENU_HAS_SCROLLBAR
    uint8_t visiable;
    vlGui_scrollBarData_t *sbData;

    sbData = &data->scrollBarData;
    visiable = vlGui_scrollBarGetVisible(sbData);
    offset = visiable * offset / (data->iconMaxSize - (2 * VLGUI_ICONMENU_ICON_MARGIN));
    offset = data->lastSbPos - offset;
    vlGui_scrollBarSetPosition(sbData, offset);

    if (!vlGui_engineIsRunning(engine)) {
        data->lastSbPos = offset;
    }
#endif

    vlGui_windowSetRefresh(win);
}

/**
 * @brief Callback for string scroll animation.
 */
static void
vlGui_iconMenuStrEngineCb(void *param, int16_t delta)
{
    vlGui_iconMenuData_t *data = (vlGui_iconMenuData_t *)param;
    data->strOffset += delta;
    vlGui_windowSetRefresh(data->win);

    if (!vlGui_engineIsRunning(&data->strEngine) && data->strOffset) {
        data->previousIndex = 0xff;
        vlGui_engineStart(&data->strEngine, -data->strOffset, VLGUI_ICONMENU_MOVE_DUR >> 1);
    }
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
void
vlGui_iconMenuDrawWin(vlGui_window_t *win, vlGui_iconMenuData_t *data, uint8_t flag)
{
    vlGui_iconMenuItem_t *selItem;
    const struct vlGui_font_t *font;
    int16_t x;
    int16_t y;
    uint16_t iconY;
    int16_t strPos;

    if (flag == VLGUI_WIN_DRAW_INIT) {
        /* Update entire window. */
    } else {
        vlGui_windowClear(win);
    }

    /* Draw a dashed line. */
    // y = VLGUI_ICONMENU_TOP_MARGIN;
    // for (int16_t i = 0; i < win->win_width - 2; i += 5) {
    //     vlGui_drawPoint(win, i, y, VLGUI_COLOR_WHITE);
    //     vlGui_drawPoint(win, i + 1, y, VLGUI_COLOR_WHITE);
    //     vlGui_drawPoint(win, i + 2, y, VLGUI_COLOR_WHITE);
    // }

    font = data->font;
    vlGui_setFont(font);

    iconY = (win->win_height - VLGUI_ICONMENU_BOTTOM_MARGIN - VLGUI_ICONMENU_TOP_MARGIN -
             font->fontHeight - VLGUI_ICONMENU_TEXT_MARGIN - data->iconMaxSize - 2) >>
            1;
    iconY += VLGUI_ICONMENU_TOP_MARGIN + 1;

    /* Draw top-left selection corner */
    x = ((win->win_width - data->iconMaxSize) >> 1) - 1 - VLGUI_ICONMENU_SEL_WID;
    y = iconY - VLGUI_ICONMENU_SEL_WID - 1;
    vlGui_drawLine(win, x, y, x + VLGUI_ICONMENU_SEL_LEN, y, VLGUI_ICONMENU_SEL_WID,
                   VLGUI_COLOR_WHITE);
    vlGui_drawLine(win, x, y, x, y + VLGUI_ICONMENU_SEL_LEN, VLGUI_ICONMENU_SEL_WID,
                   VLGUI_COLOR_WHITE);

    /* Draw bottom-right selection corner */
    x += data->iconMaxSize + 2 + VLGUI_ICONMENU_SEL_WID;
    y = iconY + data->iconMaxSize + 1;

    vlGui_drawLine(win, x, y, x - VLGUI_ICONMENU_SEL_LEN + VLGUI_ICONMENU_SEL_WID, y,
                   VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);
    vlGui_drawLine(win, x, y + 1, x, y - VLGUI_ICONMENU_SEL_LEN + VLGUI_ICONMENU_SEL_WID,
                   VLGUI_ICONMENU_SEL_WID, VLGUI_COLOR_WHITE);

    selItem = vlGui_drawIcons(win, data, iconY);

#if VLGUI_ICONMENU_HAS_SCROLLBAR
    int16_t winHight;

    winHight = win->win_height;
    /* Draw the scroll bar. Set the window hight to place the bar in the appropriate postion.
     * Then restore the window hight.
     */
    y = win->win_height - VLGUI_ICONMENU_BOTTOM_MARGIN - VLGUI_ICONMENU_TEXT_MARGIN -
        font->fontHeight - 1;
    win->win_height = y + 1;
    vlGui_scrollBarDrawWin(win, &data->scrollBarData, flag);
    win->win_height = winHight;

    /* Draw dashed line */
    y = VLGUI_ICONMENU_TOP_MARGIN;
    for (int16_t i = 0; i < win->win_width - 2; i += 5) {
        vlGui_drawPoint(win, i, y, VLGUI_COLOR_WHITE);
        vlGui_drawPoint(win, i + 1, y, VLGUI_COLOR_WHITE);
        vlGui_drawPoint(win, i + 2, y, VLGUI_COLOR_WHITE);
    }
#endif // VLGUI_ICONMENU_HAS_SCROLLBAR

    /* Draw icon name below */
    strPos = font->fontHeight + VLGUI_ICONMENU_BOTTOM_MARGIN;
    y = win->win_height - strPos;
    vlGui_drawBlock(win, 0, y, 8 - (6.0 * data->strOffset / strPos), font->fontHeight,
                    VLGUI_COLOR_WHITE);
    if (data->previousIndex != 0xff) {
        selItem = (data->previousIndex > data->selectedIndex) ? selItem->next : selItem->prev;
    }
    vlGui_drawString(win, VLGUI_STR_CENTER_X(win->win_width, font, 4), data->strOffset + y,
                     selItem->name, VLGUI_COLOR_WHITE);
}

/**
 * @brief Handle key events for the icon menu.
 */
int
vlGui_iconMenuProcessKey(vlGui_window_t *win, vlGui_iconMenuData_t *data, uint8_t key)
{
    int rc = 0;
    int16_t des;

    switch (key) {
    case VLGUI_KEY_RIGHT:
        if (data->selectedIndex >= (data->numOfItems - 1))
            break;
        data->strOffset = 0;
        data->previousIndex = data->selectedIndex;
        ++data->selectedIndex;
        des = -data->iconMaxSize - (2 * VLGUI_ICONMENU_ICON_MARGIN);
        vlGui_engineStart(&data->engine, des, VLGUI_ICONMENU_MOVE_DUR);
        des = data->font->fontHeight + VLGUI_ICONMENU_BOTTOM_MARGIN;
        vlGui_engineStart(&data->strEngine, des, VLGUI_ICONMENU_MOVE_DUR >> 1);
        vlGui_windowSetRefresh(win);
        break;

    case VLGUI_KEY_LEFT:
        if (data->selectedIndex == 0)
            break;
        data->strOffset = 0;
        data->previousIndex = data->selectedIndex;
        --data->selectedIndex;
        des = data->iconMaxSize + (2 * VLGUI_ICONMENU_ICON_MARGIN);
        vlGui_engineStart(&data->engine, des, VLGUI_ICONMENU_MOVE_DUR);
        des = data->font->fontHeight + VLGUI_ICONMENU_BOTTOM_MARGIN;
        vlGui_engineStart(&data->strEngine, des, VLGUI_ICONMENU_MOVE_DUR >> 1);
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

void
vlGui_iconMenuInitData(vlGui_window_t *win, vlGui_iconMenuData_t *data, uint8_t iconMAxSize)
{
    memset(data, 0, sizeof(vlGui_iconMenuData_t));
    data->font = vlGui_asc11x18;
    data->win = win;
    data->items = NULL;
    data->iconMaxSize = iconMAxSize;
    data->offset = (win->win_width - iconMAxSize) >> 1;
    data->lastOffset = data->offset;
    data->previousIndex = 0xff;

    vlGui_engineInit(&data->engine, vlGui_engineCurveOutQuint, vlGui_iconMenuEngineCb, data);
    vlGui_engineSetCumulative(&data->engine, true);
    vlGui_engineInit(&data->strEngine, vlGui_engineCurveOutQuint, vlGui_iconMenuStrEngineCb, data);
    win->engines = &data->engine;
    vlGui_engineAppend(&data->engine, &data->strEngine);

#if VLGUI_ICONMENU_HAS_SCROLLBAR
    vlGui_scrollBarInitData(win, &data->scrollBarData, 0);
    vlGui_scrollBarShowEndpoint(&data->scrollBarData, 0);
#endif // VLGUI_ICONMENU_HAS_SCROLLBAR
}

/**
 * @brief Create a new icon menu instance.
 */
vlGui_iconMenu_t *
vlGui_iconMenuCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width, uint16_t height,
                     uint8_t iconMAxSize)
{
    vlGui_iconMenu_t *menu;

    VLGUI_ASSERT(parent);

    menu = (vlGui_iconMenu_t *)vlGui_windowCreate(parent, x, y, width, height,
                                                  sizeof(vlGui_iconMenuData_t));
    if (!menu) {
        return NULL;
    }

    vlGui_iconMenuInitData(&menu->win, &menu->data, iconMAxSize);

    vlGui_windowSetKeyCb(&menu->win, vlGui_iconMenuProcessKeyCb);
    vlGui_windowSetDrawCb(&menu->win, vlGui_iconMenuDrawCb);
    vlGui_windowSetRefresh(&menu->win);

    return menu;
}

/**
 * @brief Add an icon item to the icon menu.
 */
void
vlGui_iconMenuAddItem(vlGui_iconMenuData_t *data, vlGui_iconMenuItem_t *item)
{
    uint8_t num;
    vlGui_iconMenuItem_t *last = data->items;

    VLGUI_ASSERT(item);

    if (last) {
        while (last->next) {
            last = last->next;
        }
        last->next = item;
        item->prev = last;
    } else {
        data->items = item;
        item->prev = NULL;
    }

    item->next = NULL;
    num = data->numOfItems + 1;
    data->numOfItems = num;

#if VLGUI_ICONMENU_HAS_SCROLLBAR
    uint8_t visiable;
    visiable = 0xff / num;
    vlGui_scrollBarSetRange(&data->scrollBarData, visiable * num);
    vlGui_scrollBarSetVisible(&data->scrollBarData, visiable);
#endif // VLGUI_ICONMENU_HAS_SCROLLBAR
}

/**
 * @brief Set the font for the icon menu.
 */
void
vlGui_iconMenuSetFont(vlGui_iconMenuData_t *data, const struct vlGui_font_t *font)
{
    data->font = font;
}