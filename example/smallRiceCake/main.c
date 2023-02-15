
/**
 * @file main.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-08
 * 
 * Copyright ? 2021 - 2022 Weilong Shen (valonshen@foxmail.com)
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
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "vlGui.h"
#include "vlGui_window.h"
#include "vlGui_input.h"
#include "vlGui_port.h"
#include "vlGui_menu.h"
#include "vlGui_clock.h"
#include "vlGui_input.h"

#include "rightArrow.h"
#include "leftArrow.h"
#include "btIcon.h"

struct vlGui_t screen;
struct vlGui_window_t *mainWin;

const uint16_t bleSetStr[] = {0xb6c0, 0xc0d1, 0xe8c9, 0xc3d6, 0x0000};
const uint16_t bleStr[] = {0xb6c0, 0xc0d1, 0xe8c9, 0xc3d6, 0x0000};
const uint16_t smartSpeakerStr[] = {0xc7d6, 0xdcc4, 0xf4d2, 0xeccf, 0x0000};
const uint16_t onStr[] = {0xf2b4, 0xaabf, 0x0000};
const uint16_t offStr[] = {0xd8b9, 0xd5b1, 0x0000};


static void
menuBarDrawCb(struct vlGui_window_t *win, void *arg)
{
    // vlGui_drawBlock(win, 0, 0, win->win_width, 10, VLGUI_COLOR_WHITE);
    vlGui_drawRectangle(win, 110, 1, 12, 7, VLGUI_COLOR_WHITE);
    vlGui_drawLine(win, 109, 3, 109, 5, 2, VLGUI_COLOR_WHITE);

    for (uint8_t i = 0; i < 3; i++) {
        vlGui_drawLine(win, 112 + (i * 3), 3, 
                         112 + (i * 3), 5, 2, VLGUI_COLOR_WHITE);
    }

    for (uint8_t i = 0; i < 3; i++) {
        vlGui_drawLine(win, 100 - (i * 2), 7 - (i * 2), 
                         100 + (i * 2), 7 - (i * 2), 
                         1, VLGUI_COLOR_WHITE);
    }
}

static void 
mainWindowDrawCb(struct vlGui_window_t *win, void *arg)
{
    vlGui_windowClear(win);

    vlGui_drawBitmap(win, 0, 10, 20, 30, leftArrow);
    vlGui_drawBitmap(win, 108, 10, 20, 30, rightArrow);
    vlGui_drawBitmap(win, 49, 0, 30, 30, btIcon);

    vlGui_setFont(vlGui_wenquan_9pt);
    vlGui_drawString(win, 40, 35, (char *)bleSetStr, VLGUI_COLOR_WHITE);
}

static int 
btSettingProcessKeyCb(struct vlGui_window_t *win, uint8_t key)
{
    struct vlGui_menu_t *menu;

    menu = (struct vlGui_menu_t *)win;
    vlGui_menuProcessKey(&menu->win, key);
    if (strcmp(vlGui_menuGetSelectedEntry(menu), "ON") == 0) {
        if (key == VLGUI_KEY_OK) {
            vlGui_menuSetFont(menu, &vlGui_font11x18);
        }
    }
    if (strcmp(vlGui_menuGetSelectedEntry(menu), "OFF") == 0) {
        if (key == VLGUI_KEY_OK) {
            vlGui_menuSetFont(menu, &vlGui_font7x10);
        }
    }
}


int 
mainWindowProcessKeyCb(struct vlGui_window_t *win, uint8_t key)
{
    struct vlGui_menu_t *menu;

    switch (key)
    {
    case VLGUI_KEY_OK:
        menu = vlGui_menuCreate(win, 0, 0, win->win_width, win->win_height);
        if (!menu) {
            break;
        }
        vlGui_windowSetKeyCb(&menu->win, btSettingProcessKeyCb);

        vlGui_menuSetFont(menu, vlGui_wenquan_9pt);
        vlGui_menuAddEntry(menu, 0, 0, (char *)bleStr);
        vlGui_menuAddEntry(menu, 1, 1, (char *)onStr);
        vlGui_menuAddEntry(menu, 2, 1, (char *)offStr);
        vlGui_menuAddEntry(menu, 3, 0, (char *)smartSpeakerStr);
        vlGui_menuAddEntry(menu, 4, 1, (char *)onStr);
        vlGui_menuAddEntry(menu, 5, 1, (char *)offStr);
        break;
    
    default:
        break;
    }
    return 0;
}

void 
main(void)
{
    struct vlGui_window_t *win;

    memset(&screen, 0, sizeof(screen));

    vlGui_inputInit();

    vlGui_screen_init(&screen, 128, 64);
    vlGui_register_driver(&screen, vlGui_portGetDriver());

    mainWin = vlinGui_getMainWindow(&screen);
    vlGui_windowSetDrawCb(mainWin, menuBarDrawCb);

    // vlGui_clockCreate(mainWin, 0, 10, mainWin->win_width, mainWin->win_height - 10);

    // vlGui_menuCreate(mainWin, 0, 10, mainWin->win_width, mainWin->win_height - 10);

    win = vlGui_windowCreate(mainWin, 0, 10, mainWin->win_width, mainWin->win_height - 10, 0);
    vlGui_windowSetDrawCb(win, mainWindowDrawCb);
    vlGui_windowSetKeyCb(win, mainWindowProcessKeyCb);

    while (1) {
        vlGui_refresh();
        vlGui_delay(20);
    }
}