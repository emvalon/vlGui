/**
 * @file vlGui_button.c
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
#include "vlGui_button.h"
#include "vlGui_window.h"
#include "vlGui_base.h"
#include <string.h>


static int vlGui_buttonProcessKey(struct vlGui_window_t *win, uint8_t key)
{
    struct vlGui_button_t *btn;

    btn = (struct vlGui_button_t *)win;
    btn->pressed ^= 1;

    return 0;
}

static void vlGui_drawButton(struct vlGui_window_t *win, void *arg)
{
    struct vlGui_button_t *btn;
    int16_t w,h, sx, sy;

    btn = (struct vlGui_button_t *)win;

    w = win->win_width;
    h = win->win_height;

    vlGui_windowClear(win);
    if(btn->pressed) {
        // vlGui_drawRectangle(win, 0, 0, w , h, 0);
        vlGui_drawBlock(win, 2, 2, w - 3, h - 3, 1);
    } else {
        vlGui_drawRectangle(win, 1, 1, w - 3, h - 3, 1);
        // 绘制下阴影
        vlGui_drawLine(win, 2, h - 2, w - 2, h - 2, 1, 1);
        // 绘制右阴影
        vlGui_drawLine(win, w - 2, 2, w - 2, h - 3, 1, 1);
    }

    vlGui_setFont(btn->font);

    sx = (w - (btn->font->fontWidth * strlen(btn->text))) >> 1;
    sy = (h - btn->font->fontHeight) >> 1;
    if (btn->pressed) {
        ++sy;
    }
    vlGui_drawString(win, sx, sy, btn->text, !btn->pressed);
}

struct vlGui_button_t *
vlGui_buttonCreate(struct vlGui_window_t *parent, int16_t x, int16_t y,     
                     int16_t width, uint16_t height)
{
    struct vlGui_button_t *btn;
    VLGUI_ASSERT(parent);

    btn = (struct vlGui_button_t *)vlGui_windowCreate(parent, x, y,
                            width, height, sizeof(*btn) - sizeof(*parent));
    if(!btn) {
        return btn;
    }

    btn->win.type = VLGUI_WIN_TYPE_BUTTON;
    btn->win.pDrawWindow = vlGui_drawButton;
    btn->win.pProcessKey = vlGui_buttonProcessKey;
    btn->pressed = 0;
    btn->text = "OK";
    btn->font = &vlGui_font7x10;

    return btn;
}

void
vlGui_buttonSetText(struct vlGui_button_t *btn, char *text)
{
    btn->text = text;
}

void
vloonGui_buttonSetFont(struct vlGui_button_t *btn, const struct vlGui_font_t *font)
{
    btn->font = font;
}