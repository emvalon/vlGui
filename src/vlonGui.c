/**
 * @file vlonGui.c
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
#include "vlonGui.h"
#include "vlonGui_input.h"
#include <string.h>

struct vlonGui_t *vlonGui_cur_screen;
static void *g_vlonGui_semphr = NULL;

int vlonGui_screen_init(struct vlonGui_t *screen, int16_t width, int16_t height)
{
    struct vlonGui_window_t *win;

    memset(screen, 0, sizeof(*screen));
    screen->refresh = 1;
    screen->width   = width;
    screen->height  = height;
    vlonGui_cur_screen = screen;

    win = vlonGui_windowCreate(NULL, 0, 0, width, height, 0);
    if(!win) {
        return -1;
    }

    vlonGui_cur_screen->window = win;

    return 0;
}


void vlonGui_register_driver(struct vlonGui_t *screen, struct vlonGui_driver_t *driver)
{
    screen->displayDriver = driver;
    driver->pInit(1);
}

static void vlonGui_refreshAllChildren(struct vlonGui_window_t *win)
{
    struct vlonGui_window_t *child;

    child = win->child;

    while (child) {
        vlonGui_windowRefresh(child);
        vlonGui_refreshAllChildren(child);
        child = child->next;
    }
}

static void vlonGui_refreshAllBrothers(struct vlonGui_window_t *win)
{
    struct vlonGui_window_t *next;

    next = win->next;

    while (next) {
        if (next->refresh) {
            vlonGui_windowRefresh(next);
        }
        next = next->next;
    }
}

void vlonGui_refresh(void)
{
    uint8_t key;
    struct vlonGui_window_t *win;
    struct vlonGui_window_t *drawWin;
    struct vlonGui_window_t *parent;

    parent = NULL;

    /* Process all of keys enqueued */
    while(1) {
        /* Get the top layer of dispaly window */
        for(win = vlonGui_cur_screen->window; win->child; parent = win, win = win->child);
        key = vlonGui_inputGetKey();

        if(key == VLGUI_KEY_NONE) {
            break;
        } else if (key == VLGUI_KEY_ESC) {
            vlonGui_windowDeleteChildren(parent);
        } else if (win->pProcessKey) {
            win->pProcessKey(win, key);
        }
    }

    win->refresh = 1;

    /* Check if this screen need to be refreshed.
     * Traverse all of windows that created on this screen.
     */
    if(vlonGui_cur_screen->refresh) {
        // vlonGui_cur_screen->refresh = 0;        
        drawWin = vlonGui_cur_screen->window;
        while (drawWin) {
            /* If this is the top layer, check if need bokeh */
            if ((drawWin == win) &&
                (win->win_height != vlonGui_cur_screen->height) && 
                (win->win_width != vlonGui_cur_screen->width)) {
                vlonGui_windowBokeh(vlonGui_cur_screen->window);
            }

            /* According to refresh flag, draw coresponding window */
            if (drawWin->refresh) {
                vlonGui_windowRefresh(drawWin);
            }
            vlonGui_refreshAllBrothers(drawWin);   
            drawWin = drawWin->child;
        }
        vlonGui_cur_screen->displayDriver->pFresh();
    }
}

struct vlonGui_window_t * vlinGui_getMainWindow(struct vlonGui_t *screen)
{
    return screen->window;
}

void vlonGui_setFont(const struct vlonGui_font_t *font)
{
    vlonGui_cur_screen->curFont = font;
}

void vlonGui_turnOnOff(struct vlonGui_t *screen, uint8_t display)
{
    screen->displayDriver->pInit(display);
}

void
vlonGui_lock(uint8_t en)
{

}
