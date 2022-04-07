#include "vlonGui.h"
#include "vlonGui_msgBox.h"
#include "vlonGui_base.h"
#include <string.h>

static void
vlonGui_drawMsgBox(struct vlonGui_window_t *win, void *arg)
{
    int16_t bw, bh;
    int16_t x, y;
    struct vlonGui_msgBox_t * msgbox;

    msgbox = (struct vlonGui_msgBox_t *)win;

    // 设置按钮大小
    bw = 30;
    bh = 12;

    // 计算按钮位置，使其位于右侧中间位置
    x = 3 * (win->win_width >> 2) - (bw >> 1);
    y = win->win_height - bh - 5;

    vlonGui_drawBlock(win, 0, 0, win->win_width, win->win_height, 0);
    // 绘制msgbox的外边框
    vlonGui_drawRectangle(win, 1, 1, win->win_width - 2, win->win_height - 2, 1);
    vlonGui_drawLine(win, 1, 11, win->win_width - 2, 11, 1, 1);
    vlonGui_drawLine(win, 3, 12, win->win_width - 2, 12, 1, 1);

    // 绘制按键边框，下阴影，右阴影
    vlonGui_drawRectangle(win, x, y, bw, bh, 1);
    vlonGui_drawLine(win, x + 1, y + bh, x + bw, y + bh, 1, 1);
    vlonGui_drawLine(win, x + bw, y + 1, x + bw, y + bh, 1, 1);

    vlonGui_setFont(&vlonGui_font6x8);
    vlonGui_drawString(win, x + 9, y + 2, "OK", 1);
    
    // 绘制title
    if(msgbox->title) {
        x = win->win_width - (strlen(msgbox->title) * vlonGui_font6x8.FontWidth);
        x >>= 1;
        vlonGui_drawString(win, x, 3, msgbox->title, 1);
    }
    // 绘制主体文本
    if(msgbox->text) {
        vlonGui_drawString(win, 2, 20, msgbox->text, 1);
    }
}

struct vlonGui_msgBox_t *
vlonGui_msgBoxCreate(struct vlonGui_window_t *parent)
{
    uint16_t w,h;
    struct vlonGui_msgBox_t * msgbox;

    VLGUI_ASSERT(parent);

    w = 90;
    h = 55;

    msgbox = (struct vlonGui_msgBox_t *)vlonGui_windowCreate(parent, 
                                (parent->win_width - w) >> 1, 
                                (parent->win_height - h) >> 1,
                                w, h, sizeof(*msgbox) - sizeof(*parent));
    if(!msgbox) {
        return msgbox;
    }

    msgbox->win.type = VLGUI_WIN_TYPE_MSGBOX;
    msgbox->win.pDrawWindow = vlonGui_drawMsgBox;
    msgbox->title = "messageBox";
    msgbox->text  = "add Test";

    return msgbox;
}

void
vlonGui_msgBoxSetProcessCb(struct vlonGui_msgBox_t *msgbox, processCb_func_t func)
{
    msgbox->pProcessCb = func;
}

void
vlonGui_msgBoxSetTitle(struct vlonGui_msgBox_t *msgbox, char *str)
{
    msgbox->title = str;
}

void
vlonGui_msgBoxSetText(struct vlonGui_msgBox_t *msgbox, char *str)
{
    msgbox->text = str;
}