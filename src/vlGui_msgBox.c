/**
 * @file vlGui_msgBox.c
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
#include "vlGui_msgBox.h"
#include "vlGui_base.h"
#include <string.h>

static void
vlGui_drawMsgBox(vlGui_window_t *win, uint8_t flag)
{
    int16_t bw, bh;
    int16_t x, y;
    struct vlGui_msgBox_t * msgbox;

    VLGUI_UNUSED(flag);
    msgbox = (struct vlGui_msgBox_t *)win;

    // 设置按钮大小
    bw = 30;
    bh = 12;

    // 计算按钮位置，使其位于右侧中间位置
    x = 3 * (win->win_width >> 2) - (bw >> 1);
    y = win->win_height - bh - 5;

    vlGui_drawBlock(win, 0, 0, win->win_width, win->win_height, 0);
    // 绘制msgbox的外边框
    vlGui_drawRectangle(win, 1, 1, win->win_width - 2, win->win_height - 2, 1);
    vlGui_drawLine(win, 1, 11, win->win_width - 2, 11, 1, 1);
    vlGui_drawLine(win, 3, 12, win->win_width - 2, 12, 1, 1);

    // 绘制按键边框，下阴影，右阴影
    vlGui_drawRectangle(win, x, y, bw, bh, 1);
    vlGui_drawLine(win, x + 1, y + bh, x + bw, y + bh, 1, 1);
    vlGui_drawLine(win, x + bw, y + 1, x + bw, y + bh, 1, 1);

    vlGui_setFont(&vlGui_font6x8);
    vlGui_drawString(win, x + 9, y + 2, "OK", 1);
    
    // 绘制title
    if(msgbox->title) {
        x = win->win_width - (strlen(msgbox->title) * vlGui_font6x8.fontWidth);
        x >>= 1;
        vlGui_drawString(win, x, 3, msgbox->title, 1);
    }
    // 绘制主体文本
    if(msgbox->text) {
        vlGui_drawString(win, 2, 20, msgbox->text, 1);
    }
}

struct vlGui_msgBox_t *
vlGui_msgBoxCreate(vlGui_window_t *parent)
{
    uint16_t w,h;
    struct vlGui_msgBox_t * msgbox;

    VLGUI_ASSERT(parent);

    w = 90;
    h = 55;

    msgbox = (struct vlGui_msgBox_t *)vlGui_windowCreate(parent, 
                                (parent->win_width - w) >> 1, 
                                (parent->win_height - h) >> 1,
                                w, h, sizeof(*msgbox) - sizeof(*parent));
    if(!msgbox) {
        return msgbox;
    }

    msgbox->win.type = VLGUI_WIN_TYPE_MSGBOX;
    msgbox->win.pDrawWindow = vlGui_drawMsgBox;
    msgbox->title = "messageBox";
    msgbox->text  = "add Test";

    return msgbox;
}

void
vlGui_msgBoxSetProcessCb(struct vlGui_msgBox_t *msgbox, processCb_func_t func)
{
    msgbox->pProcessCb = func;
}

void
vlGui_msgBoxSetTitle(struct vlGui_msgBox_t *msgbox, char *str)
{
    msgbox->title = str;
}

void
vlGui_msgBoxSetText(struct vlGui_msgBox_t *msgbox, char *str)
{
    msgbox->text = str;
}