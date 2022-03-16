#ifndef _VLONGUI_MSGBOX_H_
#define _VLONGUI_MSGBOX_H_

#include "vlonGui_window.h"

typedef void (* processCb_func_t)(uint8_t ok);

struct vlonGui_msgBox_t {
    struct vlonGui_window_t win;

    char *title;
    char *text;
    processCb_func_t pProcessCb;
};

struct vlonGui_msgBox_t * vlonGui_msgBoxCreate(struct vlonGui_window_t *parent);

void vlonGui_msgBoxSetTitle(struct vlonGui_msgBox_t *msgbox, char *str);

void vlonGui_msgBoxSetText(struct vlonGui_msgBox_t *msgbox, char *str);

#endif