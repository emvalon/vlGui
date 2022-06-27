/**
 * @file vlonGui_button.c
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
#include "vlonGui.h"
#include "vlonGui_button.h"
#include "vlonGui_window.h"
#include "vlonGui_base.h"
#include <string.h>


static int vlonGui_buttonProcessKey(struct vlonGui_window_t *win, uint8_t key)
{
    struct vlonGui_button_t *btn;

    btn = (struct vlonGui_button_t *)win;
    btn->pressed ^= 1;

    return 0;
}

static void vlonGui_drawButton(struct vlonGui_window_t *win, void *arg)
{
    struct vlonGui_button_t *btn;
    int16_t w,h, sx, sy;

    btn = (struct vlonGui_button_t *)win;

    w = win->win_width;
    h = win->win_height;

    vlonGui_windowClear(win);
    if(btn->pressed) {
        // vlonGui_drawRectangle(win, 0, 0, w , h, 0);
        vlonGui_drawBlock(win, 2, 2, w - 3, h - 3, 1);
    } else {
        vlonGui_drawRectangle(win, 1, 1, w - 3, h - 3, 1);
        // 绘制下阴影
        vlonGui_drawLine(win, 2, h - 2, w - 2, h - 2, 1, 1);
        // 绘制右阴影
        vlonGui_drawLine(win, w - 2, 2, w - 2, h - 3, 1, 1);
    }

    vlonGui_setFont(btn->font);

    sx = (w - (btn->font->FontWidth * strlen(btn->text))) >> 1;
    sy = (h - btn->font->FontHeight) >> 1;
    if (btn->pressed) {
        ++sy;
    }
    vlonGui_drawString(win, sx, sy, btn->text, !btn->pressed);
}

struct vlonGui_button_t *
vlonGui_buttonCreate(struct vlonGui_window_t *parent, int16_t x, int16_t y,     
                     int16_t width, uint16_t height)
{
    struct vlonGui_button_t *btn;
    VLGUI_ASSERT(parent);

    btn = (struct vlonGui_button_t *)vlonGui_windowCreate(parent, x, y,
                            width, height, sizeof(*btn) - sizeof(*parent));
    if(!btn) {
        return btn;
    }

    btn->win.type = VLGUI_WIN_TYPE_BUTTON;
    btn->win.pDrawWindow = vlonGui_drawButton;
    btn->win.pProcessKey = vlonGui_buttonProcessKey;
    btn->pressed = 0;
    btn->text = "OK";
    btn->font = &vlonGui_font7x10;

    return btn;
}

void
vlonGui_buttonSetText(struct vlonGui_button_t *btn, char *text)
{
    btn->text = text;
}

void
vloonGui_buttonSetFont(struct vlonGui_button_t *btn, struct vlonGui_font_t *font)
{
    btn->font = font;
}