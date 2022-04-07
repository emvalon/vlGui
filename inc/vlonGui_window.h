#ifndef _VLONGUI_WINDOW_H_
#define _VLONGUI_WINDOW_H_

#include <stdint.h>


#define VLGUI_WIN_TYPE_BASE         (0)
#define VLGUI_WIN_TYPE_MSGBOX       (1)
#define VLGUI_WIN_TYPE_BUTTON       (2)
#define VLGUI_WIN_TYPE_SELECTOR     (3)



struct vlonGui_window_t {
    uint8_t type:4;
    uint8_t animation:1;

    int16_t x1, y1;
    int16_t x2, y2;
    int16_t win_width;
    int16_t win_height;
    /* Used for animation */
    int16_t x_offset;
    int16_t y_offset;
    int16_t ani_dx;
    int16_t ani_dy;
    int16_t ori_x_offset;
    int16_t ori_y_offset;
    uint16_t start_time;
    uint16_t ani_ms;
    void (*pAnimatinDone)(void *);
    void *animatinDoneArg;

    struct vlonGui_window_t *child;
    struct vlonGui_window_t *next;

    void (* pDrawWindow)(struct vlonGui_window_t *win, void *arg);
    int (* pProcessKey)(struct vlonGui_window_t *win, uint8_t key);
};

typedef void (* vlonGui_drawWindowCb_t)(struct vlonGui_window_t *win, void *arg);
typedef int (* vlonGui_processKeyCb_t)(struct vlonGui_window_t *win, uint8_t key);
typedef void (* vlonGui_animationDoneCb)(void *arg);

struct vlonGui_window_t * vlonGui_windowCreate(struct vlonGui_window_t *parent, int16_t x, int16_t y, 
                                               int16_t width, int16_t height, uint8_t userDataLen);


void vlonGui_windowDelete(struct vlonGui_window_t *win);

void vlonGui_windowDeleteChildren(struct vlonGui_window_t *win);

void vlonGui_windowFresh(struct vlonGui_window_t *win);

void vlonGui_windowSetDrawCb(struct vlonGui_window_t *win, vlonGui_drawWindowCb_t func);

void vlonGui_windowMove(struct vlonGui_window_t *win, int16_t dx, int16_t dy);

void vlonGui_windowClear(struct vlonGui_window_t *win);

void vlonGui_windowScroll(struct vlonGui_window_t *win, int16_t dx, int16_t dy);

void vlonGui_windowSetKeyCb(struct vlonGui_window_t *win, vlonGui_processKeyCb_t func);

void vlonGui_windowScrollAnimation(struct  vlonGui_window_t *win, int16_t dx, int16_t dy, 
                                   uint16_t ms, vlonGui_animationDoneCb cb, void *arg);

#endif