/**
 * @file vlonGui_window.c
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
#include "vlonGui.h"
#include "vlonGui_window.h"
#include "vlonGui_base.h"
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
 * @return struct vlonGui_window_t* 
 */
struct vlonGui_window_t *
vlonGui_windowCreate(struct vlonGui_window_t *parent, int16_t x, int16_t y, 
                     int16_t width, int16_t height, uint8_t userDataLen)
{
    struct vlonGui_window_t *win;
    struct vlonGui_window_t *brother;

    win = vlonGui_malloc(sizeof(*win) + userDataLen);
    if(!win) {
        return win;
    }

    memset(win, 0, sizeof(*win));

    if(parent) {
        win->x1 = parent->x1 + x;
        win->y1 = parent->y1 + y;
    } else {
        win->x1 = x;
        win->y1 = y;
    }
    win->x2 = win->x1 + width -1;
    win->y2 = win->y1 + height -1;
    win->x_offset = 0;
    win->y_offset = 0;
    win->pDrawWindow = (void *)0;
    win->win_width = width;
    win->win_height = height;

    win->type = VLGUI_WIN_TYPE_BASE;
    
    if(parent) {
        brother = parent->child;
        if (brother) {
            for (; brother->next; brother = brother->next);
            brother->next = win;
        } else {
            parent->child = win;
        }
    }

    return win;
}

void
vlonGui_windowDelete(struct vlonGui_window_t *win)
{
    struct vlonGui_window_t *child;
    struct vlonGui_window_t *next;

    child = win->child;

    while (child) {
        next = child->next;
        vlonGui_windowDelete(child);
        child = next;
    }
    
    vlonGui_free(win);
}

void
vlonGui_windowDeleteChildren(struct vlonGui_window_t *win)
{
    struct vlonGui_window_t *child;
    struct vlonGui_window_t *next;

    child = win->child;
    
    while (child) {
        next = child->next;
        vlonGui_windowDelete(child);
        child = next;
    }
    
    win->child = NULL;
}

static void vlonGui_checkAnimation(struct vlonGui_window_t *win)
{
    int16_t dx, dy;
    uint16_t dt;
    uint16_t now;

    if(win->animation == 0) {
        return;
    }

    now = vlonGui_getTime();

    if(now >= (win->start_time + win->ani_ms)) {
        win->x_offset = win->ori_x_offset + win->ani_dx;
        win->y_offset = win->ori_y_offset + win->ani_dy;
        win->animation = 0;
        if(win->pAnimatinDone) {
            win->pAnimatinDone(win->animatinDoneArg);
        }
        return;
    }

    dt = now - win->start_time;
    dx = dt * win->ani_dx / win->ani_ms ;
    dy = dt * win->ani_dy / win->ani_ms ;

    win->x_offset = win->ori_x_offset + dx;
    win->y_offset = win->ori_y_offset + dy;
} 

void vlonGui_windowBokeh(struct vlonGui_window_t *win)
{
    int pos;

    pos = 0;
    for (int16_t y = 0; y < win->win_height; y+=VLGUI_BOKEH_SIZE) {
        pos = pos ? 0 : VLGUI_BOKEH_SIZE;
        for (int x = pos; x < win->win_width; x+=(VLGUI_BOKEH_SIZE * 2)) {
            vlonGui_drawBlock(win, x - win->y_offset, y - win->x_offset, 
                              VLGUI_BOKEH_SIZE, VLGUI_BOKEH_SIZE, 0);
        }
    }
}

void vlonGui_windowFresh(struct vlonGui_window_t *win)
{
    vlonGui_checkAnimation(win);

    if(win->pDrawWindow) {
        win->pDrawWindow(win, 0);
    }
}

void vlonGui_windowSetDrawCb(struct vlonGui_window_t *win, vlonGui_drawWindowCb_t func)
{
    win->pDrawWindow = func;
}

void vlonGui_windowMove(struct vlonGui_window_t *win, int16_t dx, int16_t dy)
{
    win->x1 += dx;
    win->y1 += dy;
}

void vlonGui_windowClear(struct vlonGui_window_t *win)
{
    int16_t x, y;

    x = -win->x_offset;
    y = -win->y_offset;

    vlonGui_drawBlock(win, x, y, win->win_width, win->win_height, 0);
}

void vlonGui_windowScroll(struct vlonGui_window_t *win, int16_t dx, int16_t dy)
{
    win->x_offset += dx;
    win->y_offset += dy;
}

void vlonGui_windowSetKeyCb(struct vlonGui_window_t *win, vlonGui_processKeyCb_t func)
{
    win->pProcessKey = func;
}

void vlonGui_windowScrollAnimation(struct  vlonGui_window_t *win, int16_t dx, int16_t dy, 
                                   uint16_t ms, vlonGui_animationDoneCb cb, void *arg)
{
    if(win->animation) {
        win->ani_dx = win->ori_x_offset + win->ani_dx + dx - win->x_offset;
        win->ani_dy = win->ori_y_offset + win->ani_dy + dy - win->y_offset;
    } else {
        win->animation = 1;
        win->ani_dx = dx;
        win->ani_dy = dy;
    }
    win->ori_x_offset = win->x_offset;
    win->ori_y_offset = win->y_offset;
    win->start_time = vlonGui_getTime();
    win->ani_ms     = ms;
    win->pAnimatinDone = cb;
    win->animatinDoneArg = arg;
}