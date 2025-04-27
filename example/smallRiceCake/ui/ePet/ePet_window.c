#include "string.h"
#include "ePet.h"

#include "vlGui_engine.h"

#include "pictures/ePet/bitmap/vlGui_bitmapShirop.h"
#include "pictures/ePet/bitmap/vlGui_bitmapShirop5.h"
#include "pictures/ePet/bitmap/vlGui_bitmapShirop3.h"
#include "pictures/ePet/bitmap/vlGui_bitmapShirop4.h"
#include "pictures/ePet/bitmap/vlGui_bitmapDog.h"
#include "pictures/ePet/bitmap/vlGui_bitmapDog1.h"

struct ePet_windowData {
    vlGui_engine_t engine;
};
typedef struct ePet_windowData ePet_windowData_t;

struct ePet_window {
    vlGui_window_t win;
    ePet_windowData_t data;
};
typedef struct ePet_window ePet_window_t;

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */
static void
ePet_windowDrawCb(vlGui_window_t *win, uint8_t flag)
{

    if (flag == VLGUI_WIN_DRAW_INIT) {
        vlGui_windowClear(win);
    }
    vlGui_drawBitmap(win, 0, 0, 8, 8, vlGui_bitmapShirop);
    // vlGui_drawBitmap(win, 20, 0, 30, 30, vlGui_bitmapShirop5);
    vlGui_drawBitmap(win, 60, 0, 15, 15, vlGui_bitmapShirop3);
    vlGui_drawBitmap(win, 80, 0, 30, 30, vlGui_bitmapShirop4);

    vlGui_windowRectArea_t rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = 0xff;
    rect.hight = 0xff;
    vlGui_drawBitmapInRect(win, &rect, vlGui_bitmapDog1);

    rect.x = 30;
    vlGui_drawBitmapInRect(win, &rect, vlGui_bitmapDog);
}

static int
ePet_windowProcessKeyCb(vlGui_window_t *win, uint8_t key)
{
    VLGUI_UNUSED(win);

    switch (key) {
    default:
        return -1;
    }
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
vlGui_window_t *
ePet_windowCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width, int16_t height)
{
    vlGui_window_t *win;
    ePet_windowData_t *data;

    win = vlGui_windowCreate(parent, x, y, width, height, sizeof(ePet_windowData_t));
    if (!win) {
        return NULL;
    }

    data = (ePet_windowData_t *)win->userData;

    memset(data, 0, sizeof(ePet_windowData_t));
    vlGui_windowSetDrawCb(win, ePet_windowDrawCb);
    vlGui_windowSetKeyCb(win, ePet_windowProcessKeyCb);

    return win;
}