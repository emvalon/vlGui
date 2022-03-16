/**
 * @file vlGui_window.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2021-11-19
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
#include "vlGui_window.h"
#include "vlGui_base.h"
#include <string.h>

/**
 * @brief Create a window basing on the parent. This window is the basic canvas
 * and all objects will be drawn on it. Other widgets inherit from window.
 * UserData will store their corresponding special data.
 *
 * @param parent
 * @param x
 * @param y
 * @param width
 * @param height
 * @param userDataLen
 * @return vlGui_window_t*
 */
vlGui_window_t *
vlGui_windowCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width, int16_t height,
                   uint8_t userDataLen)
{
    vlGui_window_t *win;

    win = vlGui_malloc(sizeof(*win) + userDataLen);
    if (!win) {
        return win;
    }

    memset(win, 0, sizeof(*win));

    if (parent) {
        win->x1 = parent->x1 + x;
        win->y1 = parent->y1 + y;
    } else {
        win->x1 = x;
        win->y1 = y;
    }
    win->x2 = win->x1 + width - 1;
    win->y2 = win->y1 + height - 1;
    win->x_offset = 0;
    win->y_offset = 0;
    win->pDrawWindow = (void *)0;
    win->win_width = width;
    win->win_height = height;

    win->type = VLGUI_WIN_TYPE_BASE;
    win->drawFlag = VLGUI_WIN_DRAW_INIT;
    win->blur = 0;
    win->refresh = 1;

    win->parent = parent;
    if (parent) {
        parent->child = win;
    }

    return win;
}

void
vlGui_windowDeleteChildren(vlGui_window_t *win)
{
    vlGui_window_t *child;
    vlGui_window_t *parent;

    child = win->child;
    if (!child) {
        return;
    }

    while (child->child) {
        child = child->child;
    }

    do {
        parent = child->parent;
        parent->child = NULL;
        vlGui_free(child);
        child = parent;
    } while (child != win);
}

static void
vlGui_checkAnimation(vlGui_window_t *win)
{
    int16_t dx, dy;
    uint16_t dt;
    uint16_t now;

    if (win->animation == 0) {
        return;
    }

    now = vlGui_getTime();

    if (now >= (win->start_time + win->ani_ms)) {
        win->x_offset = win->ori_x_offset + win->ani_dx;
        win->y_offset = win->ori_y_offset + win->ani_dy;
        win->animation = 0;
        if (win->pAnimatinDone) {
            win->pAnimatinDone(win->animatinDoneArg);
        }
        return;
    }

    dt = now - win->start_time;
    dx = dt * win->ani_dx / win->ani_ms;
    dy = dt * win->ani_dy / win->ani_ms;

    win->x_offset = win->ori_x_offset + dx;
    win->y_offset = win->ori_y_offset + dy;
}

void
vlGui_windowBlur(vlGui_window_t *win, uint8_t factor)
{
    if (factor > 3) {
        factor = 3;
    }

    for (uint16_t i = 0; i < factor; i++)
        for (uint16_t y = 0; y < win->win_height; y++)
            for (uint16_t x = 0; x < win->win_width; x++) {
                if (!(x & 0x0001) && !(y & 0x0001)) {
                    vlGui_drawPoint(win, x + (i > 0 && i < 3), y + (i > 1), VLGUI_COLOR_BLACK);
                }
            }
}

void
vlGui_windowSetDrawCb(vlGui_window_t *win, vlGui_drawWindowCb_t func)
{
    win->pDrawWindow = func;
}

void
vlGui_windowMove(vlGui_window_t *win, int16_t dx, int16_t dy)
{
    win->x1 += dx;
    win->y1 += dy;
}

void
vlGui_windowMoveTo(vlGui_window_t *win, int16_t x, int16_t y)
{
    win->x1 = x;
    win->y1 = y;
}

void
vlGui_windowClear(vlGui_window_t *win)
{
    // int16_t x, y;

    // x = -win->x_offset;
    // y = -win->y_offset;

    // vlGui_drawBlock(win, x, y, win->win_width, win->win_height, 0);
    vlGui_drawBlock(win, 0, 0, win->win_width, win->win_height, 0);
}

void
vlGui_windowScroll(vlGui_window_t *win, int16_t dx, int16_t dy)
{
    win->x_offset += dx;
    win->y_offset += dy;
}

void
vlGui_windowSetKeyCb(vlGui_window_t *win, vlGui_processKeyCb_t func)
{
    win->pProcessKey = func;
}

void
vlGui_windowScrollAnimation(vlGui_window_t *win, int16_t dx, int16_t dy, uint16_t ms,
                            vlGui_animationDoneCb cb, void *arg)
{
    if (win->animation) {
        win->ani_dx = win->ori_x_offset + win->ani_dx + dx - win->x_offset;
        win->ani_dy = win->ori_y_offset + win->ani_dy + dy - win->y_offset;
    } else {
        win->animation = 1;
        win->ani_dx = dx;
        win->ani_dy = dy;
    }
    win->ori_x_offset = win->x_offset;
    win->ori_y_offset = win->y_offset;
    win->start_time = vlGui_getTime();
    win->ani_ms = ms;
    win->pAnimatinDone = cb;
    win->animatinDoneArg = arg;
}

void
vlGui_windowSetRefresh(vlGui_window_t *win)
{
    win->refresh = 1;
}

void
vlGui_windowBlurEnable(vlGui_window_t *win, bool enable)
{
    win->blur = enable;
}

void
vlGui_windowBackgroundUpdate(vlGui_window_t *win, bool enable)
{
    win->backgroundUpdate = enable;
}
