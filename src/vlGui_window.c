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
 * @return struct vlGui_window_t* 
 */
struct vlGui_window_t *
vlGui_windowCreate(struct vlGui_window_t *parent, int16_t x, int16_t y, 
                     int16_t width, int16_t height, uint8_t userDataLen)
{
    struct vlGui_window_t *win;
    struct vlGui_window_t *brother;

    win = vlGui_malloc(sizeof(*win) + userDataLen);
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
    win->refresh = 1;
    
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
vlGui_windowDelete(struct vlGui_window_t *win)
{
    struct vlGui_window_t *child;
    struct vlGui_window_t *next;

    child = win->child;

    while (child) {
        next = child->next;
        vlGui_windowDelete(child);
        child = next;
    }
    
    vlGui_free(win);
}

void
vlGui_windowDeleteChildren(struct vlGui_window_t *win)
{
    struct vlGui_window_t *child;
    struct vlGui_window_t *next;

    child = win->child;
    
    while (child) {
        next = child->next;
        vlGui_windowDelete(child);
        child = next;
    }
    
    win->child = NULL;
}

static void vlGui_checkAnimation(struct vlGui_window_t *win)
{
    int16_t dx, dy;
    uint16_t dt;
    uint16_t now;

    if(win->animation == 0) {
        return;
    }

    now = vlGui_getTime();

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

void vlGui_windowBokeh(struct vlGui_window_t *win)
{
    int pos;

    pos = 0;
    for (int16_t y = 0; y < win->win_height; y+=VLGUI_BOKEH_SIZE) {
        pos = pos ? 0 : VLGUI_BOKEH_SIZE;
        for (int x = pos; x < win->win_width; x+=(VLGUI_BOKEH_SIZE * 2)) {
            vlGui_drawBlock(win, x - win->y_offset, y - win->x_offset, 
                              VLGUI_BOKEH_SIZE, VLGUI_BOKEH_SIZE, 0);
        }
    }
}

void 
vlGui_windowRefresh(struct vlGui_window_t *win)
{
    win->refresh = 0;
    
    vlGui_checkAnimation(win);
    vlGui_engineRender(win->engines, (void *)win);

    if(win->pDrawWindow) {
        win->pDrawWindow(win, 0);
    }
}

void vlGui_windowSetDrawCb(struct vlGui_window_t *win, vlGui_drawWindowCb_t func)
{
    win->pDrawWindow = func;
}

void vlGui_windowMove(struct vlGui_window_t *win, int16_t dx, int16_t dy)
{
    win->x1 += dx;
    win->y1 += dy;
}

void vlGui_windowClear(struct vlGui_window_t *win)
{
    int16_t x, y;

    x = -win->x_offset;
    y = -win->y_offset;

    vlGui_drawBlock(win, x, y, win->win_width, win->win_height, 0);
}

void vlGui_windowScroll(struct vlGui_window_t *win, int16_t dx, int16_t dy)
{
    win->x_offset += dx;
    win->y_offset += dy;
}

void vlGui_windowSetKeyCb(struct vlGui_window_t *win, vlGui_processKeyCb_t func)
{
    win->pProcessKey = func;
}

void vlGui_windowScrollAnimation(struct  vlGui_window_t *win, int16_t dx, int16_t dy, 
                                   uint16_t ms, vlGui_animationDoneCb cb, void *arg)
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
    win->start_time = vlGui_getTime();
    win->ani_ms     = ms;
    win->pAnimatinDone = cb;
    win->animatinDoneArg = arg;
}

void
vlGui_windowInitEngine(vlGui_engine_t *engine)
{
    
}

void
vlGui_windowSetRefresh(struct vlGui_window_t * win)
{
    win->refresh = 1;
}
