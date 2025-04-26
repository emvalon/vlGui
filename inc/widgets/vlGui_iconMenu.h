/**
 * @file vlGui_iconMenu.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-14
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
#ifndef _VLGUI_ICONMENU_H_
#define _VLGUI_ICONMENU_H_

#include <stdint.h>
#include "vlGui.h"
#include "vlGui_window.h"
#include "vlGui_input.h"
#include "fonts/vlGui_fonts.h"
#include "vlGui_engine.h"
#include "vlGui_scrollBar.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VLGUI_ICONMENU_HAS_SCROLLBAR (0)

/**
 * @brief Structure defining an item in the icon menu.
 */
typedef struct vlGui_iconMenuItem_t {
    char *name;
    const uint8_t *icon;
    uint8_t iconWidth;
    uint8_t iconHeight;
    struct vlGui_iconMenuItem_t *prev;
    struct vlGui_iconMenuItem_t *next;
} vlGui_iconMenuItem_t;

struct vlGui_iconMenuData {
    const struct vlGui_font_t *font;
    vlGui_window_t *win;
    vlGui_iconMenuItem_t *items;
    uint8_t selectedIndex;
    uint8_t numOfItems;
    uint8_t iconMaxSize;
    uint8_t previousIndex;

    int16_t offset;
    int16_t lastOffset;
    vlGui_engine_t engine;
    vlGui_engine_t strEngine;
    int16_t strOffset;

#if VLGUI_ICONMENU_HAS_SCROLLBAR
    int16_t lastSbPos;
    vlGui_scrollBarData_t scrollBarData;
#endif // VLGUI_ICONMENU_HAS_SCROLLBAR
};
typedef struct vlGui_iconMenuData vlGui_iconMenuData_t;
/**
 * @brief Structure defining the icon menu.
 */
typedef struct {
    vlGui_window_t win;
    vlGui_iconMenuData_t data;
} vlGui_iconMenu_t;

/**
 * @brief Create a new icon menu instance.
 */
vlGui_iconMenu_t *vlGui_iconMenuCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width,
                                       uint16_t height, uint8_t iconMaxSize);

/**
 * @brief Add an icon item to the icon menu.
 */
void vlGui_iconMenuAddItem(vlGui_iconMenuData_t *menuData, vlGui_iconMenuItem_t *item);

/**
 * @brief Set the font for the icon menu.
 */
void vlGui_iconMenuSetFont(vlGui_iconMenuData_t *menuData, const struct vlGui_font_t *font);

/**
 * @brief Handle key events for the icon menu.
 */
int vlGui_iconMenuProcessKey(vlGui_window_t *win, vlGui_iconMenuData_t *data, uint8_t key);

void vlGui_iconMenuDrawWin(vlGui_window_t *win, vlGui_iconMenuData_t *pData, uint8_t flag);

void vlGui_iconMenuDrawWin(vlGui_window_t *win, vlGui_iconMenuData_t *data, uint8_t flag);

#ifdef __cplusplus
}
#endif

#endif // _VLGUI_ICONMENU_H_