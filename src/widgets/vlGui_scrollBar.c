
/**
 * @file vlGui_scrollBar.c
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
#include <stdio.h>
#include "vlGui.h"
#include "vlGui_base.h"
#include "widgets/vlGui_scrollBar.h"

#define VLGUI_SCROLLBAR_LS_MARGIN (2)
#define VLGUI_SCROLLBAR_SS_MARGIN (1)
#define VLGUI_SCROLLBAR_THICKNESS (3)
#define VLGUI_SCROLLBAR_INSET (1)

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */

/*
 * Callback function to draw the scrollbar window.
 *
 * @param win Window handle
 * @param arg Draw state flag
 */
static void
vlGui_scrollBarDrawCb(vlGui_window_t *win, uint8_t arg)
{
    if (arg == VLGUI_WIN_DRAW_INIT) {
        vlGui_windowClear(win);
    }
    vlGui_scrollBarDrawWin(win, (vlGui_scrollBarData_t *)win->userData, arg);
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */

/*
 * Render the scrollbar inside a window.
 * Supports both vertical and horizontal modes.
 *
 * @param win Window handle
 * @param data Pointer to scrollbar data
 * @param flag Draw state flag
 */
void
vlGui_scrollBarDrawWin(vlGui_window_t *win, vlGui_scrollBarData_t *data, uint8_t flag)
{
    int16_t x, y, length, sliderLength, sliderPos;
    uint16_t indicatorEp;

    if (data->isVertical) {
        x = win->win_width - VLGUI_SCROLLBAR_LS_MARGIN;
        y = VLGUI_SCROLLBAR_SS_MARGIN;
        length = win->win_height - 2 * VLGUI_SCROLLBAR_SS_MARGIN;
    } else {
        x = VLGUI_SCROLLBAR_SS_MARGIN;
        y = win->win_height - VLGUI_SCROLLBAR_LS_MARGIN;
        length = win->win_width - 2 * VLGUI_SCROLLBAR_LS_MARGIN;
    }

    if (data->isVertical) {
        indicatorEp = x - VLGUI_SCROLLBAR_THICKNESS + 2;
        /* Draw vertical lines */
        vlGui_drawBlock(win, indicatorEp, y + 1, VLGUI_SCROLLBAR_THICKNESS, length - 2,
                        VLGUI_COLOR_BLACK);
        vlGui_drawLine(win, x, y + 1, x, y + length - 2, 1, VLGUI_COLOR_WHITE);
        /* Draw the endpoint lines */
        if ((flag == VLGUI_WIN_DRAW_INIT) && (data->drawEndpoint)) {
            vlGui_drawLine(win, indicatorEp, y, indicatorEp + VLGUI_SCROLLBAR_THICKNESS, y, 1,
                           VLGUI_COLOR_WHITE);
            vlGui_drawLine(win, indicatorEp, y + length - 1,
                           indicatorEp + VLGUI_SCROLLBAR_THICKNESS, y + length - 1, 1,
                           VLGUI_COLOR_WHITE);
        }
    } else {
        indicatorEp = y - VLGUI_SCROLLBAR_THICKNESS + 2;
        vlGui_drawBlock(win, x + 1, indicatorEp, length - 2, VLGUI_SCROLLBAR_THICKNESS,
                        VLGUI_COLOR_BLACK);
        vlGui_drawLine(win, x, y, x + length - 2, y, 1, VLGUI_COLOR_WHITE);
        /* Draw the endpoint lines */
        if ((flag == VLGUI_WIN_DRAW_INIT) && (data->drawEndpoint)) {
            vlGui_drawLine(win, x, indicatorEp, x, indicatorEp + VLGUI_SCROLLBAR_THICKNESS, 1,
                           VLGUI_COLOR_WHITE);
            vlGui_drawLine(win, x + length - 1, indicatorEp, x + length - 1,
                           indicatorEp + VLGUI_SCROLLBAR_THICKNESS, 1, VLGUI_COLOR_WHITE);
        }
    }

    if (data->totalCount == 0 || data->visibleCount == 0 ||
        data->visibleCount >= data->totalCount) {
        return;
    }

    /* Avoid division by using multiplication and shifting when possible */
    sliderLength = (int32_t)length * data->visibleCount / data->totalCount;
    sliderPos = (int32_t)length * data->position / data->totalCount;

    --length;
    if (sliderPos + sliderLength > length) {
        sliderPos = length - sliderLength;
    }
    /* Draw the current slider indicator */
    if (data->isVertical) {
        vlGui_drawBlock(win, indicatorEp, y + sliderPos, VLGUI_SCROLLBAR_THICKNESS, sliderLength,
                        VLGUI_COLOR_WHITE);
    } else {
        vlGui_drawBlock(win, x + sliderPos, indicatorEp, sliderLength, VLGUI_SCROLLBAR_THICKNESS,
                        VLGUI_COLOR_WHITE);
    }
}

void
vlGui_scrollBarSetRange(vlGui_scrollBarData_t *data, uint8_t range)
{
    data->totalCount = range;
}

uint8_t
vlGui_scrollBarGetRange(vlGui_scrollBarData_t *data)
{
    return data->totalCount;
}

void
vlGui_scrollBarSetVisible(vlGui_scrollBarData_t *data, uint8_t visible)
{
    data->visibleCount = visible;
}

uint8_t
vlGui_scrollBarGetVisible(vlGui_scrollBarData_t *data)
{
    return data->visibleCount;
}

void
vlGui_scrollBarSetPosition(vlGui_scrollBarData_t *data, uint8_t pos)
{
    if (pos <= data->totalCount) {
        data->position = pos;
    }
}

uint8_t
vlGui_scrollBarGetPosition(vlGui_scrollBarData_t *data)
{
    return data->position;
}

void
vlGui_scrollBarShowEndpoint(vlGui_scrollBarData_t *data, uint8_t show)
{
    data->drawEndpoint = show;
}

/*
 * Initialize scrollbar data with default values.
 *
 * @param win Window handle
 * @param data Pointer to scrollbar data
 */
void
vlGui_scrollBarInitData(vlGui_window_t *win, vlGui_scrollBarData_t *data, uint8_t isVertical)
{
    VLGUI_UNUSED(win);
    data->totalCount = 0;
    data->visibleCount = 0;
    data->position = 0;
    data->isVertical = isVertical;
    data->drawEndpoint = 1;
}

/*
 * Create scrollbar widget window.
 *
 * @param parent Parent window
 * @param x X position
 * @param y Y position
 * @param width Width of the scrollbar window
 * @param height Height of the scrollbar window
 * @param isVertical 1 for vertical, 0 for horizontal
 * @return Pointer to scrollbar widget
 */
vlGui_scrollBar_t *
vlGui_scrollBarCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width, uint16_t height,
                      uint8_t isVertical)
{
    vlGui_scrollBar_t *sb;

    sb = (vlGui_scrollBar_t *)vlGui_windowCreate(parent, x, y, width, height,
                                                 sizeof(vlGui_scrollBarData_t));
    if (!sb) {
        return NULL;
    }

    vlGui_scrollBarInitData(&sb->win, &sb->data, isVertical);
    vlGui_windowSetDrawCb(&sb->win, vlGui_scrollBarDrawCb);

    return sb;
}

/*
 * Destroy the scrollbar window.
 *
 * @param sb Pointer to scrollbar instance
 */
void
vlGui_scrollBarDestroy(vlGui_scrollBar_t *sb)
{
    VLGUI_UNUSED(sb);
}