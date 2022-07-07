
/**
 * @file vlonGui_fonts.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-09
 * 
 * Copyright © 2019-2022 Weilong Shen (valonshen@foxmail.com)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "vlonGui.h"
#include "vlonGui_msgBox.h"
#include "vlonGui_window.h"
#include "vlonGui_input.h"
#include "vlonGui_button.h"
#include "vlonGui_selector.h"
#include "vlonGui_progressBar.h"
#include "bitmap.h"
#include "games.h"

struct vlonGui_t screen;
struct vlonGui_msgBox_t *msgbox;
struct vlonGui_window_t *mainWin, *win, *topWin;
struct vlonGui_button_t *btn;
struct vlonGui_selector_t *sel;
struct vlonGui_progressBar_t *progBar;

static bool showIconName = true;
volatile int8_t pos = 0;


static uint8_t presskey = VLGUI_KEY_RIGHT;

static void 
mainWindowDrawCb(struct vlonGui_window_t *win, void *arg)
{
    int16_t x;
    
    vlonGui_windowClear(win);
    x = 12;
    vlonGui_drawBitmap(win, x, 4, 40, 40, bitmap_game);
    x += 128;
    vlonGui_drawBitmap(win, x, 4, 40, 40, bitmap_setting);
    x += 128;
    vlonGui_drawBitmap(win, x, 4, 40, 40, bitmap_connect);
    x += 128;
    vlonGui_drawBitmap(win, x, 4, 40, 40, bitmap_info);
    x += 128;
    vlonGui_drawBitmap(win, x, 4, 40, 40, bitmap_media);
}

static void 
topWindowDrawCb(struct vlonGui_window_t *win, void *arg)
{
    uint8_t w,h;
    int16_t x;
    char *str;

    w = 10;
    h = 5;
    x = (128 - 5 * w) >> 1;

    for(uint8_t i = 0; i < 5; i++) {
        if(i == pos) {  
            vlonGui_drawBlock(win, x + i * w, 64 - h, w, h, 1);
        } else {
            vlonGui_drawRectangle(win, x + i * w, 64 - h, w, h, 1);
        }
    }

    vlonGui_drawLine(win, 63, 4, 63, 44, 2, 1);

    if(!showIconName) {
        return;
    }

    vlonGui_setFont(&vlonGui_font11x18);
    switch (pos)
    {
    case 0:
        str = "Game";
        break;
    case 1:
        str = "Set";
        break;
    case 2:
        str = "Conn";
        break;
    case 3:
        str = "Info";
        break;
    case 4:
        str = "Media";
        break;
    default:
        break;
    }

    x = 96 - ((strlen(str) * vlonGui_font11x18.FontWidth) >> 1);

    vlonGui_drawString(win, x, 20, str, 1);
}

static void 
drawIconName(void *arg)
{
   showIconName = true;
}

int 
mainWindowProcessKeyCb(struct vlonGui_window_t *win, uint8_t key)
{
    switch (key)
    {
    case VLGUI_KEY_LETF:
        if(pos) {
            --pos;
            showIconName = false;
            vlonGui_windowScrollAnimation(win, 128, 0, 500, drawIconName, NULL);
        } else {
            presskey = VLGUI_KEY_RIGHT;
        }
        
        break;
    case VLGUI_KEY_RIGHT:
        if(pos < 4) {
            ++pos;
            showIconName = false;
            vlonGui_windowScrollAnimation(win, -128, 0, 500, drawIconName, NULL);
        } else {
            presskey = VLGUI_KEY_LETF;
        }
        break;
    case VLGUI_KEY_OK:
        switch (pos)
        {
        case 0: 
            games_createSelector(win);

            break;
        case 1:
            btn = vlonGui_buttonCreate(win, 40, 20, 30, 16);
            break;
        case 2:
            // sel = vlonGui_selectorCreate(win);
            // vlonGui_selectorAddEntry(sel, "Shen Weilong");
            // vlonGui_selectorAddEntry(sel, "Valon Shen");
            // vlonGui_selectorAddEntry(sel, "Ma Suhong");
            // vlonGui_selectorAddEntry(sel, "Suhon Ma");
            // vlonGui_selectorAddEntry(sel, "VlonGui");
            msgbox = vlonGui_msgBoxCreate(win);
            break;
        case 3:
            progBar = vlonGui_progressBarCreate(win, 10, 15, 108, 34);
            vlonGui_progressBarSetValue(progBar, 60);
            vlonGui_progressBarSetTitle(progBar, "ProgressBar");
        default:
            break;
        }
    default:
        break;
    }

    return 0;
}

void 
main(void)
{
    memset(&screen, 0, sizeof(screen));

    vlonGui_inputInit();

    vlonGui_screen_init(&screen, 128, 64);
    vlonGui_register_driver(&screen, vlonGui_portGetDriver());

    mainWin = vlinGui_getMainWindow(&screen);

    win = vlonGui_windowCreate(mainWin, 0, 0, mainWin->win_width, mainWin->win_height, 0);
    vlonGui_windowSetDrawCb(win, mainWindowDrawCb);
    vlonGui_windowSetKeyCb(win, mainWindowProcessKeyCb);

    topWin = vlonGui_windowCreate(mainWin, 0, 0, win->win_width, win->win_height, 0);
    vlonGui_windowSetDrawCb(topWin, topWindowDrawCb);

    // msgbox = vlonGui_msgBoxCreate(vlinGui_getMainWindow(&screen));
    // vlonGui_msgBoxSetTitle(msgbox, "TestTitle");
    // vlonGui_msgBoxSetText(msgbox, "Warning: test");

    while (1) {
        vlonGui_refresh();
        usleep(30*1000);
    }
}