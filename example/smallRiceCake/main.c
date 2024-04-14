
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
#include "vlGui_input.h"
#include "ui.h"

#define STATUS_BAR_HEIGHT            (10)

static void
menuBarDrawCb(vlGui_window_t *win, uint8_t flag)
{
    VLGUI_UNUSED(flag);

    vlGui_windowClear(win);
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

int 
main(void)
{
    static struct vlGui_t screen;
    vlGui_window_t *mainWin;

    vlGui_init();
    
    /* Initialize the screen with 128x64 pixels */
    vlGui_screen_init(&screen, 128, 64);
    /* Set the dispaly driver. For multiple screans, we can set different drivers for them. */
    vlGui_register_driver(&screen, (struct vlGui_driver_t *)&vlGui_driverGtk);

    mainWin = vlinGui_getMainWindow(&screen);
    vlGui_windowSetDrawCb(mainWin, menuBarDrawCb);
    ui_homeWinCreate(mainWin, 0, STATUS_BAR_HEIGHT, mainWin->win_width, 
                     mainWin->win_height - STATUS_BAR_HEIGHT);

    /* Fresh the screen with about 30 fps */
    while (1) {
        vlGui_refresh();
        vlGui_delay(33);
    }

    return 0;
}