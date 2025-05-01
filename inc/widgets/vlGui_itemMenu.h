/**
 * @file vlGui_itemMenu.h
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
#ifndef _VLGUI_ITEMMENU_H_
#define _VLGUI_ITEMMENU_H_

#include <stdint.h>
#include "vlGui_window.h"
#include "vlGui_input.h"
#include "vlGui_engine.h"
#include "vlGui_scrollBar.h"

struct vlGui_itemMenuEntry {
    const char *text;
    struct vlGui_itemMenuEntry *next;
};
typedef struct vlGui_itemMenuEntry vlGui_itemMenuEntry_t;

typedef int (*vlGui_itemMenuDrawEntryCb_t)(vlGui_window_t *win, vlGui_windowRectArea_t *area,
                                           vlGui_itemMenuEntry_t *entry);

struct vlGui_itemMenuData {
    uint8_t clear : 1;
    int16_t offset;

    vlGui_itemMenuEntry_t *entries;
    const struct vlGui_font_t *font;
    vlGui_itemMenuDrawEntryCb_t entryDrawCb;
    vlGui_engine_t engine;
    vlGui_engineCurve_t engineCurve;
    vlGui_scrollBarData_t sbData;
};
typedef struct vlGui_itemMenuData vlGui_itemMenuData_t;

struct vlGui_itemMenu {
    vlGui_window_t win;
    vlGui_itemMenuData_t data;
};
typedef struct vlGui_itemMenu vlGui_itemMenu_t;

/**
 * @brief Callback function for progress bar to process coming key.
 *
 * @param win
 * @param key
 * @return int
 */
int vlGui_itemMenuProcessKey(vlGui_window_t *win, vlGui_itemMenuData_t *data, uint8_t key);

void vlGui_itemMenuDrawWin(vlGui_window_t *win, vlGui_itemMenuData_t *pbarData, uint8_t flag);

void vlGui_itemMenuInitData(vlGui_window_t *win, vlGui_itemMenuData_t *pMenuData,
                            const struct vlGui_font_t *font, vlGui_engineCurve_t engineCurve);

vlGui_itemMenu_t *vlGui_itemMenuCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width,
                                       uint16_t height);

void vlGui_itemMenuAddEntry(vlGui_itemMenuData_t *pMenuData, vlGui_itemMenuEntry_t *entry);

void vlGui_itemMenuSetEntryDrawCb(vlGui_itemMenuData_t *pMenuData, vlGui_itemMenuDrawEntryCb_t cb);

#endif // _VLGUI_ITEMMENU_H_