/**
 * @file vlGui_window.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-04-21
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
#ifndef _VLGUI_WINDOW_H_
#define _VLGUI_WINDOW_H_

#include <stdint.h>
#include "vlGui_engine.h"

#define VLGUI_WIN_TYPE_BASE (0)
#define VLGUI_WIN_TYPE_MSGBOX (1)
#define VLGUI_WIN_TYPE_BUTTON (2)
#define VLGUI_WIN_TYPE_SELECTOR (3)
#define VLGUI_WIN_TYPE_PROGBAR (4)
#define VLGUI_WIN_TYPE_MENU (5)

#define VLGUI_WIN_DRAW_INIT (0)
#define VLGUI_WIN_DRAW_REFRESH (1)
#define VLGUI_WIN_DRAW_BACKGROUND (2)

struct vlGui_window {
    uint8_t type : 4;
    uint8_t animation : 1;
    uint8_t refresh : 1;
    uint8_t backgroundUpdate : 1;
    uint8_t blur : 1;

    uint8_t drawFlag;
    int16_t x1, y1;
    int16_t x2, y2;
    int16_t win_width;
    int16_t win_height;
    /* Used for animation */
    int16_t x_offset;
    int16_t y_offset;
    int16_t ani_dx;
    int16_t ani_dy;
    int16_t ori_x_offset;
    int16_t ori_y_offset;
    uint16_t start_time;
    uint16_t ani_ms;
    void (*pAnimatinDone)(void *);
    void *animatinDoneArg;

    vlGui_engine_t *engines;

    struct vlGui_window *parent;
    struct vlGui_window *child;

    void (*pDrawWindow)(struct vlGui_window *win, uint8_t arg);
    int (*pProcessKey)(struct vlGui_window *win, uint8_t key);

    void *userData[0];
};
typedef struct vlGui_window vlGui_window_t;

typedef void (*vlGui_drawWindowCb_t)(vlGui_window_t *win, uint8_t arg);
typedef int (*vlGui_processKeyCb_t)(vlGui_window_t *win, uint8_t key);
typedef void (*vlGui_animationDoneCb)(void *arg);

vlGui_window_t *vlGui_windowCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width,
                                   int16_t height, uint8_t userDataLen);

void vlGui_windowDeleteChildren(vlGui_window_t *win);

void vlGui_windowBlur(vlGui_window_t *win, uint8_t factor);

void vlGui_windowSetRefresh(vlGui_window_t *win);

void vlGui_windowSetDrawCb(vlGui_window_t *win, vlGui_drawWindowCb_t func);

void vlGui_windowMove(vlGui_window_t *win, int16_t dx, int16_t dy);

void vlGui_windowMoveTo(vlGui_window_t *win, int16_t x, int16_t y);

void vlGui_windowClear(vlGui_window_t *win);

void vlGui_windowScroll(vlGui_window_t *win, int16_t dx, int16_t dy);

void vlGui_windowSetKeyCb(vlGui_window_t *win, vlGui_processKeyCb_t func);

void vlGui_windowScrollAnimation(vlGui_window_t *win, int16_t dx, int16_t dy, uint16_t ms,
                                 vlGui_animationDoneCb cb, void *arg);

void vlGui_windowBlurEnable(vlGui_window_t *win, bool enable);

void vlGui_windowBackgroundUpdate(vlGui_window_t *win, bool enable);

#endif