/**
 * @file vlGui.c
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
#include "vlGui.h"
#include "vlGui_input.h"
#include <string.h>

struct vlGui_t *vlGui_cur_screen;
static void *g_vlGui_semphr = NULL;

int vlGui_screen_init(struct vlGui_t *screen, int16_t width, int16_t height)
{
    struct vlGui_window_t *win;

    memset(screen, 0, sizeof(*screen));
    screen->refresh = 1;
    screen->width   = width;
    screen->height  = height;
    vlGui_cur_screen = screen;

    win = vlGui_windowCreate(NULL, 0, 0, width, height, 0);
    if(!win) {
        return -1;
    }

    vlGui_cur_screen->window = win;

    return 0;
}


void vlGui_register_driver(struct vlGui_t *screen, struct vlGui_driver_t *driver)
{
    screen->displayDriver = driver;
    driver->pInit(1);
}

static void vlGui_refreshAllChildren(struct vlGui_window_t *win)
{
    struct vlGui_window_t *child;

    child = win->child;

    while (child) {
        vlGui_windowRefresh(child);
        vlGui_refreshAllChildren(child);
        child = child->next;
    }
}

static void vlGui_refreshAllBrothers(struct vlGui_window_t *win)
{
    struct vlGui_window_t *next;

    next = win->next;

    while (next) {
        if (next->refresh) {
            vlGui_windowRefresh(next);
        }
        next = next->next;
    }
}

void vlGui_refresh(void)
{
    uint8_t key;
    struct vlGui_window_t *win;
    struct vlGui_window_t *drawWin;
    struct vlGui_window_t *parent;

    parent = NULL;

    /* Process all of keys enqueued */
    while(1) {
        /* Get the top layer of dispaly window */
        for(win = vlGui_cur_screen->window; win->child; parent = win, win = win->child);
        key = vlGui_inputGetKey();

        if(key == VLGUI_KEY_NONE) {
            break;
        } else if (key == VLGUI_KEY_ESC) {
            vlGui_windowDeleteChildren(parent);
        } else if (win->pProcessKey) {
            win->pProcessKey(win, key);
        }
    }

    win->refresh = 1;

    /* Check if this screen need to be refreshed.
     * Traverse all of windows that created on this screen.
     */
    if(vlGui_cur_screen->refresh) {
        // vlGui_cur_screen->refresh = 0;        
        drawWin = vlGui_cur_screen->window;
        while (drawWin) {
            /* If this is the top layer, check if need bokeh */
            if ((drawWin == win) &&
                (win->win_height != vlGui_cur_screen->height) && 
                (win->win_width != vlGui_cur_screen->width)) {
                vlGui_windowBokeh(vlGui_cur_screen->window);
            }

            /* According to refresh flag, draw coresponding window */
            if (drawWin->refresh) {
                vlGui_windowRefresh(drawWin);
            }
            vlGui_refreshAllBrothers(drawWin);   
            drawWin = drawWin->child;
        }
        vlGui_cur_screen->displayDriver->pFresh();
    }
}

struct vlGui_window_t * vlinGui_getMainWindow(struct vlGui_t *screen)
{
    return screen->window;
}

void vlGui_setFont(const struct vlGui_font_t *font)
{
    vlGui_cur_screen->curFont = font;
}

void vlGui_turnOnOff(struct vlGui_t *screen, uint8_t display)
{
    screen->displayDriver->pInit(display);
}

void
vlGui_lock(uint8_t en)
{

}
