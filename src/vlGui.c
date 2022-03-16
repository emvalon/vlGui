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

#define VLGUI_SWITCH_ANIMATION_CALC_BITS (7)
#define VLGUI_SWITCH_ANIMATION_TIME_US (400)

struct vlGui_t *vlGui_cur_screen;

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */
static void
vlGui_refreshWindow(vlGui_window_t *win, bool isTop)
{
    uint8_t drawFlags;

    drawFlags = win->drawFlag;
    if (drawFlags == VLGUI_WIN_DRAW_INIT) {
        /* If the new window can not fill the entire screen and needed, try to achieve
         * ground glass effect.
         */
        if (!isTop && win->blur) {
            vlGui_windowBlur(win->parent, 3);
        }
        win->drawFlag = VLGUI_WIN_DRAW_REFRESH;
    }

    vlGui_engineRender(win->engines);

    if (win->pDrawWindow) {
        win->pDrawWindow(win, drawFlags);
    }
}

static void
vlGui_refreshScreen(vlGui_window_t *firstWin, vlGui_window_t *secWin)
{
    if (firstWin->refresh) {
        // firstWin->refresh = 0;
        vlGui_refreshWindow(firstWin, true);
    }
    if (secWin && secWin->refresh) {
        // secWin->refresh = 0;
        vlGui_refreshWindow(secWin, false);
    }
    vlGui_cur_screen->displayDriver->pFresh();
}

int
vlGui_screen_init(struct vlGui_t *screen, int16_t width, int16_t height)
{
    vlGui_window_t *win;

    memset(screen, 0, sizeof(*screen));
    screen->refresh = 1;
    screen->width = width;
    screen->height = height;
    vlGui_cur_screen = screen;

    win = vlGui_windowCreate(NULL, 0, 0, width, height, 0);
    if (!win) {
        return -1;
    }

    vlGui_cur_screen->window = win;
    vlGui_cur_screen->topWin = win;

    vlGui_engineInit(&vlGui_cur_screen->switchEngine,
                     vlGui_engineCurveInOutQuart,
                     vlGui_switchEffectRight2LeftCb,
                     NULL);
    vlGui_engineSetCumulative(&vlGui_cur_screen->switchEngine, true);

    return 0;
}

void
vlGui_register_driver(struct vlGui_t *screen, struct vlGui_driver_t *driver)
{
    screen->displayDriver = driver;
    driver->pInit(1);
}

void
vlGui_refresh(void)
{
    int rc;
    uint8_t key;
    vlGui_window_t *win;
    vlGui_window_t *topWin;
    vlGui_engine_t *switchEngine;

    /* If the switch animation is ongoing, don't respond to any input keys currently. */
    switchEngine = &vlGui_cur_screen->switchEngine;
    if (vlGui_engineIsRunning(switchEngine)) {
        vlGui_engineRender(switchEngine);
        return;
    }

    /* Get the top layer of dispaly window. If there are new windows created, start the switch
     * animation.
     */
    topWin = vlGui_cur_screen->topWin;
    if (topWin->child) {
        win = topWin;
        while (win->child) {
            win = win->child;
        }
        vlGui_engineSetParam(switchEngine, (void *)win);
        vlGui_engineStart(
            switchEngine, 1 << VLGUI_SWITCH_ANIMATION_CALC_BITS, VLGUI_SWITCH_ANIMATION_TIME_US);
        return;
    }

    /* Process all of keys enqueued */
    while (true) {
        key = vlGui_inputGetKey();
        if (key == VLGUI_KEY_NONE) {
            break;
        }
        rc = topWin->pProcessKey(topWin, key);
        if (rc) {
            win = topWin->parent;
            if (!win) {
                continue;
            }
            vlGui_engineSetParam(switchEngine, (void *)win);
            vlGui_engineStart(switchEngine,
                              1 << VLGUI_SWITCH_ANIMATION_CALC_BITS,
                              VLGUI_SWITCH_ANIMATION_TIME_US);
            return;
        }
    }

    if (vlGui_cur_screen->refresh) {
        // vlGui_cur_screen->refresh = 0;
        vlGui_refreshScreen(topWin, NULL);
    }
}

vlGui_window_t *
vlinGui_getMainWindow(struct vlGui_t *screen)
{
    return screen->window;
}

void
vlGui_setFont(const struct vlGui_font_t *font)
{
    vlGui_cur_screen->curFont = font;
}

void
vlGui_turnOnOff(struct vlGui_t *screen, uint8_t display)
{
    screen->displayDriver->pInit(display);
}

/*
 **************************************************************************************************
 * Switching Effect Functions
 **************************************************************************************************
 */
void
vlGui_switchEffectRight2LeftCb(void *param, int16_t delta)
{
    int32_t offset;
    vlGui_window_t *win;
    vlGui_window_t *topWin;

    win = (vlGui_window_t *)param;
    topWin = vlGui_cur_screen->topWin;

    /* This new windows will be shown with animation. Therefore, set corresponding
     * flags to redraw all of content.
     */
    win->drawFlag = VLGUI_WIN_DRAW_INIT;
    vlGui_windowSetRefresh(win);

    /* `topWin->child == NULL` means there is no new windows created. The topWin was deleted. */
    if (topWin->child) {
        offset = (delta * win->win_width) >> VLGUI_SWITCH_ANIMATION_CALC_BITS;
        vlGui_windowMoveTo(win, 0 + (win->win_width - offset), win->y1);

        if (!vlGui_engineIsRunning(&vlGui_cur_screen->switchEngine)) {
            vlGui_cur_screen->topWin = win;
            vlGui_refreshScreen(win, NULL);
        } else {
            vlGui_refreshScreen(topWin, win);
        }
    } else {
        offset = (delta * topWin->win_width) >> VLGUI_SWITCH_ANIMATION_CALC_BITS;
        vlGui_windowMoveTo(topWin, 0 + offset, topWin->y1);

        if (!vlGui_engineIsRunning(&vlGui_cur_screen->switchEngine)) {
            /* Delete all of children and free corresponding memory. */
            vlGui_windowDeleteChildren(win);
            vlGui_cur_screen->topWin = win;
            vlGui_refreshScreen(win, NULL);
        } else {
            /* The `topWin1 will be removed. */
            vlGui_windowSetRefresh(topWin);
            topWin->drawFlag = VLGUI_WIN_DRAW_INIT;
            vlGui_refreshScreen(win, topWin);
        }
    }
}

void
vlGui_switchEffectLargenCb(void *param, int16_t delta)
{
    int32_t largen;
    vlGui_window_t *win;
    vlGui_window_t *topWin;

    win = (vlGui_window_t *)param;
    topWin = vlGui_cur_screen->topWin;

    /* This new windows will be shown with animation. Therefore, set corresponding
     * flags to redraw all of content.
     */
    win->drawFlag = VLGUI_WIN_DRAW_INIT;
    vlGui_windowSetRefresh(win);

    /* `topWin->child == NULL` means there is no new windows created. The topWin was deleted. */
    if (topWin->child) {
        largen = (delta * 128) >> VLGUI_SWITCH_ANIMATION_CALC_BITS;
        // vlGui_windowMoveTo(win, 0 + (win->win_width - largen), win->y1);
        win->win_width = largen;

        if (!vlGui_engineIsRunning(&vlGui_cur_screen->switchEngine)) {
            vlGui_cur_screen->topWin = win;
            vlGui_refreshScreen(win, NULL);
        } else {
            vlGui_refreshScreen(topWin, win);
        }
    } else {
        // offset = (delta * topWin->win_width) >> VLGUI_SWITCH_ANIMATION_CALC_BITS;
        // vlGui_windowMoveTo(topWin, 0 + offset, topWin->y1);

        if (!vlGui_engineIsRunning(&vlGui_cur_screen->switchEngine)) {
            /* Delete all of children and free corresponding memory. */
            vlGui_windowDeleteChildren(win);
            vlGui_cur_screen->topWin = win;
            vlGui_refreshScreen(win, NULL);
        } else {
            /* The `topWin1 will be removed. */
            vlGui_windowSetRefresh(topWin);
            topWin->drawFlag = VLGUI_WIN_DRAW_INIT;
            vlGui_refreshScreen(win, topWin);
        }
    }
}