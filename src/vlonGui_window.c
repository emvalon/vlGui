/**
 * @file vlonGui_window.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-19
 * 
 * Copyright © 2019-2021 Weilong Shen (valonshen@foxmail.com).
 * All rights reserved.
 * 
 */
#include "vlonGui.h"
#include "vlonGui_window.h"
#include "vlonGui_base.h"
#include <string.h>

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
        }

        if (brother) {
            brother->next = win;
        } else {
            parent->child = win;
        }
    }

    return win;
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
            win->pAnimatinDone();
        }
        return;
    }

    dt = now - win->start_time;
    dx = dt * win->ani_dx / win->ani_ms ;
    dy = dt * win->ani_dy / win->ani_ms ;

    win->x_offset = win->ori_x_offset + dx;
    win->y_offset = win->ori_y_offset + dy;
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
                                   uint16_t ms, vlonGui_animationDoneCb cb)
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
}