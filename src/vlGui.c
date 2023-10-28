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
#include <stdio.h>

struct vlGui_t *vlGui_cur_screen;
//TODO
// static void *g_vlGui_semphr = NULL;

static void
vlGui_switchEngineCb(void *param, int16_t delta)
{
    vlGui_window_t *win;

    win = (vlGui_window_t *)param;
    vlGui_windowScroll(win, delta, 0);
    if (!vlGui_engineIsRunning(&vlGui_cur_screen->switchEngine)) {
        
    }
}

static void 
vlGui_refreshWindow(vlGui_window_t *win, uint8_t drawFlags)
{
    vlGui_window_t *next;

    // win->refresh = 0;
    
    // vlGui_checkAnimation(win);
    vlGui_engineRender(win->engines);

    if(win->pDrawWindow) {
        win->pDrawWindow(win, drawFlags);
        next = win->next;
        while (next) {
            next->pDrawWindow(next, drawFlags);
            next = next->next;
        }
    }
}

int vlGui_screen_init(struct vlGui_t *screen, int16_t width, int16_t height)
{
    vlGui_window_t *win;

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
    vlGui_cur_screen->topWin = win;

    return 0;
}


void vlGui_register_driver(struct vlGui_t *screen, struct vlGui_driver_t *driver)
{
    screen->displayDriver = driver;
    driver->pInit(1);
}

void vlGui_refresh(void)
{
    int rc;
    uint8_t key;
    uint8_t drawFlags;
    vlGui_window_t *win;
    vlGui_window_t *parent;

    /* Get the top layer of dispaly window */
    win = vlGui_cur_screen->topWin;
    /* Process all of keys enqueued */
    while(true) {
        key = vlGui_inputGetKey();
        if(key == VLGUI_KEY_NONE) {
            break;
        } 

        rc = win->pProcessKey(win, key);
        if (rc) {
            parent = win->parent;
            if (!parent) {
                break;
            }
            /* Delete all of children and free corresponding memory. */
            vlGui_windowDeleteChildren(parent);
            parent->drawFlag = VLGUI_WIN_DRAW_INIT;
            vlGui_windowSetRefresh(parent);
            win = parent;
        }
    }

    vlGui_engineRender(&vlGui_cur_screen->switchEngine);

    // win->refresh = 1;

    /* Check if this screen need to be refreshed.
     * Traverse all of windows that created on this screen.
     */
    if(vlGui_cur_screen->refresh) {
        // vlGui_cur_screen->refresh = 0;

        /* According to refresh flag, draw corresponding window */
        if (win->refresh) {
            if ((win->blur) && (win->win_height != vlGui_cur_screen->height) && 
                (win->win_width != vlGui_cur_screen->width)) {
                vlGui_windowBlur(vlGui_cur_screen->window, 3);
            }
            drawFlags = win->drawFlag;
            if (drawFlags == VLGUI_WIN_DRAW_INIT) {
                vlGui_engineStop(&vlGui_cur_screen->switchEngine);
                vlGui_engineInit(&vlGui_cur_screen->switchEngine,
                                 vlGui_engineCurveOutQuint,
                                 vlGui_switchEngineCb, win);
                vlGui_engineStart(&vlGui_cur_screen->switchEngine, 
                                  win->win_width, 1000);
                win->drawFlag = VLGUI_WIN_DRAW_REFRESH;
            }
            vlGui_refreshWindow(win, drawFlags);
        }
    
        vlGui_cur_screen->displayDriver->pFresh();
    }






    // if(vlGui_cur_screen->refresh) {
    //     // vlGui_cur_screen->refresh = 0;        
    //     drawWin = vlGui_cur_screen->window;
    //     while (drawWin) {
    //         /* If this is the top layer, check if need blur */
    //         if (drawWin == win){
    //             background = false;
    //             if ((drawWin->blur) &&
    //                 (win->win_height != vlGui_cur_screen->height) && 
    //                 (win->win_width != vlGui_cur_screen->width)) {
    //                 vlGui_windowBlur(vlGui_cur_screen->window, 3);
    //             }
    //         }
    //         /* According to refresh flag, draw corresponding window */
    //         if (drawWin->backgroundUpdate || drawWin->refresh) {
    //             vlGui_refreshWindow(drawWin, background);
    //         }
    //         vlGui_refreshAllBrothers(drawWin, background);   
    //         drawWin = drawWin->child;
    //     }
    //     vlGui_cur_screen->displayDriver->pFresh();
    // }
}

vlGui_window_t * vlinGui_getMainWindow(struct vlGui_t *screen)
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
