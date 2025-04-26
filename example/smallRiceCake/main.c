
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
#include "ui/ui.h"
#include "fonts/vlGui_u8Fonts.h"

#define STATUS_BAR_HEIGHT (8)
#define STATUS_BAR_SPACING (0)

static void
menuBarDrawCb(vlGui_window_t *win, uint8_t flag)
{
    int16_t x;
    const struct vlGui_font_t *font;

    VLGUI_UNUSED(flag);

    vlGui_windowClear(win);
    // vlGui_drawBlock(win, 0, 0, win->win_width, 10, VLGUI_COLOR_WHITE);
    // vlGui_drawRectangle(win, 110, 1, 12, 7, VLGUI_COLOR_WHITE);
    // vlGui_drawLine(win, 109, 3, 109, 5, 2, VLGUI_COLOR_WHITE);

    font = vlGui_font_openIconicEmbeded8x8;
    vlGui_setFont(font);
    x = win->win_width;
    x -= font->fontWidth + STATUS_BAR_SPACING;
    /* Battery icon */
    vlGui_u8FontsDrawGlyph(win, x, STATUS_BAR_HEIGHT, 0x49);

    /* Bluetooth icon */
    x -= font->fontWidth + STATUS_BAR_SPACING;
    vlGui_u8FontsDrawGlyph(win, x, STATUS_BAR_HEIGHT, 0x4a);

    /* Wifi icon */
    x -= font->fontWidth + STATUS_BAR_SPACING;
    vlGui_u8FontsDrawGlyph(win, x, STATUS_BAR_HEIGHT, 0x50);

    /* warning icon */
    x -= font->fontWidth + STATUS_BAR_SPACING;
    vlGui_u8FontsDrawGlyph(win, x, STATUS_BAR_HEIGHT, 0x47);
}

int
main(void)
{
    static struct vlGui_t screen;
    vlGui_window_t *mainWin;

    memset(&screen, 0, sizeof(screen));
    vlGui_inputInit();
    /* Initialize the screen with 128x64 pixels */
    vlGui_screen_init(&screen, 128, 64);
    vlGui_register_driver(&screen, vlGui_portGetDriver());

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