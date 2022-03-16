
/**
 * @file vlGui_fonts.h
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

#include "vlGui.h"
#include "vlGui_msgBox.h"
#include "vlGui_window.h"
#include "vlGui_input.h"
#include "vlGui_button.h"
#include "vlGui_selector.h"
#include "vlGui_progressBar.h"
#include "bitmap.h"
#include "games.h"

struct vlGui_t screen;
struct vlGui_msgBox_t *msgbox;
vlGui_window_t *mainWin, *win, *topWin;
struct vlGui_button_t *btn;
struct vlGui_selector_t *sel;
struct vlGui_progressBar_t *progBar;

static bool showIconName = true;
volatile int8_t pos = 0;


static uint8_t presskey = VLGUI_KEY_RIGHT;

static void 
mainWindowDrawCb(vlGui_window_t *win, uint8_t flag)
{
    int16_t x;
    
    vlGui_windowClear(win);
    x = 12;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_game);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_setting);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_connect);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_info);
    x += 128;
    vlGui_drawBitmap(win, x, 4, 40, 40, bitmap_media);
}

static void 
topWindowDrawCb(vlGui_window_t *win, uint8_t flag)
{
    uint8_t w,h;
    int16_t x;
    char *str;

    w = 10;
    h = 5;
    x = (128 - 5 * w) >> 1;

    for(uint8_t i = 0; i < 5; i++) {
        if(i == pos) {  
            vlGui_drawBlock(win, x + i * w, 64 - h, w, h, 1);
        } else {
            vlGui_drawRectangle(win, x + i * w, 64 - h, w, h, 1);
        }
    }

    vlGui_drawLine(win, 63, 4, 63, 44, 2, 1);

    if(!showIconName) {
        return;
    }

    vlGui_setFont(&vlGui_font11x18);
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

    x = 96 - ((strlen(str) * vlGui_font11x18.fontWidth) >> 1);

    vlGui_drawString(win, x, 20, str, 1);
}

static void 
drawIconName(uint8_t flag)
{
   showIconName = true;
}

int 
mainWindowProcessKeyCb(vlGui_window_t *win, uint8_t key)
{
    switch (key)
    {
    case VLGUI_KEY_LEFT:
        if(pos) {
            --pos;
            showIconName = false;
            vlGui_windowScrollAnimation(win, 128, 0, 500, drawIconName, NULL);
        } else {
            presskey = VLGUI_KEY_RIGHT;
        }
        
        break;
    case VLGUI_KEY_RIGHT:
        if(pos < 4) {
            ++pos;
            showIconName = false;
            vlGui_windowScrollAnimation(win, -128, 0, 500, drawIconName, NULL);
        } else {
            presskey = VLGUI_KEY_LEFT;
        }
        break;
    case VLGUI_KEY_OK:
        switch (pos)
        {
        case 0: 
            games_createSelector(win);

            break;
        case 1:
            btn = vlGui_buttonCreate(win, 40, 20, 30, 16);
            break;
        case 2:
            // sel = vlGui_selectorCreate(win);
            // vlGui_selectorAddEntry(sel, "Shen Weilong");
            // vlGui_selectorAddEntry(sel, "Valon Shen");
            // vlGui_selectorAddEntry(sel, "Ma Suhong");
            // vlGui_selectorAddEntry(sel, "Suhon Ma");
            // vlGui_selectorAddEntry(sel, "VlonGui");
            msgbox = vlGui_msgBoxCreate(win);
            break;
        case 3:
            progBar = vlGui_progressBarCreate(win, 10, 15, 108, 34);
            vlGui_progressBarSetValue(progBar, 60);
            vlGui_progressBarSetTitle(progBar, "ProgressBar");
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

    vlGui_inputInit();

    vlGui_screen_init(&screen, 128, 64);
    vlGui_register_driver(&screen, vlGui_portGetDriver());

    mainWin = vlinGui_getMainWindow(&screen);

    win = vlGui_windowCreate(mainWin, 0, 0, mainWin->win_width, mainWin->win_height, 0);
    vlGui_windowSetDrawCb(win, mainWindowDrawCb);
    vlGui_windowSetKeyCb(win, mainWindowProcessKeyCb);

    topWin = vlGui_windowCreate(mainWin, 0, 0, win->win_width, win->win_height, 0);
    vlGui_windowSetDrawCb(topWin, topWindowDrawCb);

    // msgbox = vlGui_msgBoxCreate(vlinGui_getMainWindow(&screen));
    // vlGui_msgBoxSetTitle(msgbox, "TestTitle");
    // vlGui_msgBoxSetText(msgbox, "Warning: test");

    while (1) {
        vlGui_refresh();
        vlGui_delay(20);
    }
}