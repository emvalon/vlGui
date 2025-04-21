/**
 * @file vlGui_scrollBar.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-19
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
#ifndef _VLGUI_SCROLLBAR_H_
#define _VLGUI_SCROLLBAR_H_

#include <stdint.h>
#include "vlGui_window.h"
#include "vlGui_input.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Scroll bar data structure
 */
typedef struct {
    uint8_t totalCount;     /**< Total number of items */
    uint8_t visibleCount;   /**< Number of visible items */
    uint8_t position;       /**< Current scroll position */
    uint8_t isVertical : 1; /**< Scroll direction: 1 for vertical, 0 for horizontal */
    uint8_t drawEndpoint : 1;
} vlGui_scrollBarData_t;

/**
 * @brief Scroll bar widget structure
 */
typedef struct {
    vlGui_window_t win;
    vlGui_scrollBarData_t data;
} vlGui_scrollBar_t;

/**
 * @brief Initialize scroll bar data with default values
 *
 * @param win Window handle (can be NULL)
 * @param data Pointer to scrollbar data
 * @param isVertical Indicate if this bar is vertical
 */
void vlGui_scrollBarInitData(vlGui_window_t *win, vlGui_scrollBarData_t *data, uint8_t isVertical);

void vlGui_scrollBarSetRange(vlGui_scrollBarData_t *data, uint8_t range);

void vlGui_scrollBarSetVisible(vlGui_scrollBarData_t *data, uint8_t visible);

uint8_t vlGui_scrollBarGetVisible(vlGui_scrollBarData_t *data);

void vlGui_scrollBarSetPosition(vlGui_scrollBarData_t *data, uint8_t pos);

void vlGui_scrollBarShowEndpoint(vlGui_scrollBarData_t *data, uint8_t show);

/**
 * @brief Draw scrollbar contents in window
 *
 * @param win Pointer to window
 * @param data Pointer to scrollbar data
 * @param flag Draw state flag
 */
void vlGui_scrollBarDrawWin(vlGui_window_t *win, vlGui_scrollBarData_t *data, uint8_t flag);

/**
 * @brief Create a new scrollbar window
 *
 * @param parent Parent window
 * @param x X position
 * @param y Y position
 * @param width Width of the window
 * @param height Height of the window
 * @param isVertical 1 for vertical, 0 for horizontal
 * @return Pointer to scrollbar widget
 */
vlGui_scrollBar_t *vlGui_scrollBarCreate(vlGui_window_t *parent, int16_t x, int16_t y,
                                         int16_t width, uint16_t height, uint8_t isVertical);

/**
 * @brief Destroy a scrollbar widget
 *
 * @param sb Pointer to scrollbar instance
 */
void vlGui_scrollBarDestroy(vlGui_scrollBar_t *sb);

#ifdef __cplusplus
}
#endif

#endif // _VLGUI_SCROLLBAR_H_