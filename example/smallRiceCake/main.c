
/**
 * @file main.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-08
 * 
 * Copyright 漏 2021 - 2022 Weilong Shen (valonshen@foxmail.com)
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

#include "vlonGui.h"
#include "vlonGui_window.h"
#include "vlonGui_input.h"
#include "vlonGui_port.h"
#include "vlonGui_menu.h"
#include "vlonGui_input.h"

#include "rightArrow.h"
#include "leftArrow.h"
#include "btIcon.h"

extern unsigned char gbk2312[];

struct vlonGui_t screen;
struct vlonGui_window_t *mainWin;

static void 
mainWindowDrawCb(struct vlonGui_window_t *win, void *arg)
{
    vlonGui_windowClear(win);

    vlonGui_drawBitmap(win, 0, 10, 20, 30, leftArrow);
    vlonGui_drawBitmap(win, 108, 10, 20, 30, rightArrow);
    vlonGui_drawBitmap(win, 49, 0, 30, 30, btIcon);
    // vlonGui_setFont(&vlonGui_font16x26);
    // vlonGui_drawString(win, 0, 0, "test win", 1);


void vlonGui_drawUnicode(struct vlonGui_window_t *win, int16_t x, int16_t y, char *str, vlonGui_color color, void *font);
    // vlonGui_drawUnicode(win, 40, 35, "蓝", VLGUI_COLOR_WHITE, gbk2312);
    // vlonGui_drawUnicode(win, 60, 35, "牙", VLGUI_COLOR_WHITE, gbk2312);
    // printf("--len:%d\n", strlen("啊"));
    vlonGui_drawUnicode(win, 20, 35, "蓝牙连接成功", VLGUI_COLOR_WHITE, gbk2312);
}


int 
mainWindowProcessKeyCb(struct vlonGui_window_t *win, uint8_t key)
{
    struct vlonGui_menu_t *menu;

    switch (key)
    {
    case VLGUI_KEY_OK:
        menu = vlonGui_menuCreate(win, 0, 0, win->win_width, win->win_height);
        if (!menu) {
            break;
        }
        vlonGui_menuAddEntry(menu, 0, 0, "Classic BT");
        vlonGui_menuAddEntry(menu, 1, 1, "ON");
        vlonGui_menuAddEntry(menu, 2, 1, "OFF");
        vlonGui_menuAddEntry(menu, 3, 0, "BLE");
        vlonGui_menuAddEntry(menu, 4, 1, "ON");
        vlonGui_menuAddEntry(menu, 5, 1, "OFF");
        break;
    
    default:
        break;
    }
    return 0;
}

void 
main(void)
{
    struct vlonGui_window_t *win;

    char *str = "鍟?";

    for (uint16_t i = 0; i < strlen(str); i++ ){
        printf("0x%x ", str[i]);
    }
    printf("%s\n", str);

    memset(&screen, 0, sizeof(screen));

    vlonGui_inputInit();

    vlonGui_screen_init(&screen, 128, 64);
    vlonGui_register_driver(&screen, vlonGui_portGetDriver());

    mainWin = vlinGui_getMainWindow(&screen);

    // vlonGui_menuCreate(mainWin, 0, 10, mainWin->win_width, mainWin->win_height - 10);

    win = vlonGui_windowCreate(mainWin, 0, 10, mainWin->win_width, mainWin->win_height - 10, 0);
    vlonGui_windowSetDrawCb(win, mainWindowDrawCb);
    vlonGui_windowSetKeyCb(win, mainWindowProcessKeyCb);

    while (1) {
        vlonGui_refresh();
        vlonGui_delay(50);
    }
}