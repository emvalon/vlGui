/**
 * @file ui_menu.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-09
 *
 * Copyright 2021 - 2023 Weilong Shen (valonshen@foxmail.com)
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
#include <stdio.h>
#include "vlGui.h"
#include "ui/ui.h"
#include "vlGui_iconMenu.h"

#include "rightArrow.h"
#include "leftArrow.h"
#include "btIcon.h"

static const uint16_t strBleSetting_cn[] = {0xc0b6, 0xd1c0, 0xc9e8, 0xd6c3, 0x0000};
// static const uint16_t bleStr[] = {0xc0b6, 0xd1c0, 0xc9e8, 0xd6c3, 0x0000};
static const uint16_t strSmartSpeaker_cn[] = {0xd6c7, 0xc4dc, 0xd2f4, 0xcfec, 0x0000};
// static const uint16_t onStr[] = {0xb4f2, 0xbfaa, 0x0000};
// static const uint16_t offStr[] = {0xb9d8, 0xb1d5, 0x0000};

static int
ui_menuProcessKeyCb(vlGui_window_t *win, uint8_t key)
{
    VLGUI_UNUSED(win);
    VLGUI_UNUSED(key);

    return 0;
}

static const uint8_t bleIcon[] = {
    0xf8, 0xff, 0xff, 0x07, 0xfe, 0xff, 0xff, 0x1f, 0xfe, 0x9f, 0xff, 0x1f, 0xff, 0x1f, 0xff,
    0x3f, 0xff, 0x1f, 0xfc, 0x3f, 0xff, 0x1f, 0xf8, 0x3f, 0xff, 0x9f, 0xe0, 0x3f, 0xff, 0x9f,
    0xc3, 0x3f, 0x9f, 0x9f, 0x07, 0x3f, 0x0f, 0x9f, 0x1f, 0x3c, 0x3f, 0x9c, 0x0f, 0x3f, 0x7f,
    0x90, 0x83, 0x3f, 0xff, 0x81, 0xe1, 0x3f, 0xff, 0x03, 0xf8, 0x3f, 0xff, 0x0f, 0xfc, 0x3f,
    0xff, 0x0f, 0xfc, 0x3f, 0xff, 0x03, 0xf0, 0x3f, 0xff, 0x80, 0xe1, 0x3f, 0x7f, 0x98, 0x83,
    0x3f, 0x1f, 0x9c, 0x0f, 0x3e, 0x0f, 0x9f, 0x1f, 0x3e, 0x9f, 0x9f, 0x07, 0x3f, 0xff, 0x9f,
    0xc1, 0x3f, 0xff, 0x9f, 0xf0, 0x3f, 0xff, 0x1f, 0xf8, 0x3f, 0xff, 0x1f, 0xfe, 0x3f, 0xff,
    0x1f, 0xff, 0x3f, 0xfe, 0xdf, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0xff, 0x07,
};

extern const unsigned char setting[];
extern const unsigned char update[];
extern const unsigned char ble[];

static vlGui_iconMenuItem_t s_menuItems[4] = {{
                                                  .name = (char *)strBleSetting_cn,
                                                  .icon = (uint8_t *)bleIcon,
                                                  .iconWidth = 30,
                                                  .iconHeight = 30,
                                              },
                                              {
                                                  .name = (char *)strSmartSpeaker_cn,
                                                  .icon = (uint8_t *)setting,
                                                  .iconWidth = 30,
                                                  .iconHeight = 30,
                                              },
                                              {
                                                  .name = (char *)strBleSetting_cn,
                                                  .icon = (uint8_t *)update,
                                                  .iconWidth = 30,
                                                  .iconHeight = 30,
                                              },
                                              {
                                                  .name = (char *)strBleSetting_cn,
                                                  .icon = (uint8_t *)ble,
                                                  .iconWidth = 30,
                                                  .iconHeight = 30,
                                              }};

void
ui_menuWinCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width, int16_t height)
{
    vlGui_iconMenu_t *menu;

    menu = vlGui_iconMenuCreate(parent, x, y, width, height, 30);
    vlGui_iconMenuSetFont(menu, vlGui_wenquan_9pt);
    for (int i = 0; i < (int)(sizeof(s_menuItems) / sizeof(s_menuItems[0])); i++) {
        vlGui_iconMenuAddItem(menu, &s_menuItems[i]);
    }

    vlGui_windowBlurEnable(&menu->win, true);
}