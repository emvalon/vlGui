#include "string.h"
#include "ePet.h"

#include "vlGui_engine.h"
#include "fonts/vlGui_u8Fonts.h"

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */
// #include "pictures/ePet/inc/vlGui_bmpBaby1.h"
// #include "pictures/ePet/inc/vlGui_bmpBaby1_gloomy.h"

#include "pictures/ePet/inc/vlGui_bmpBlood.h"
#include "pictures/ePet/inc/vlGui_bmpBlood.h"
#include "pictures/ePet/inc/vlGui_bmpBloodEmpty.h"
#include "pictures/ePet/inc/vlGui_bmpHeart.h"
#include "pictures/ePet/inc/vlGui_bmpHeartEmpty.h"
#include "pictures/ePet/inc/vlGui_bmpFood.h"
#include "pictures/ePet/inc/vlGui_bmpFoodEmpty.h"
#include "pictures/ePet/inc/vlGui_bmpEducation.h"
#include "pictures/ePet/inc/vlGui_bmpEducationEmpty.h"

// static const uint8_t ePent_bmpHight[EPENT_GROWTH_MAX] = {
//     15,
//     20,
//     25,
//     30,
// };

// static const uint8_t ePent_bmpWidth[EPENT_GROWTH_MAX] = {
//     15,
//     20,
//     25,
//     30,
// };

static void
ePet_windowDrawCb(vlGui_window_t *win, uint8_t flag)
{
    int16_t dx;
    int16_t w;
    // int16_t y;
    ePet_windowData_t *data;
    // vlGui_windowRectArea_t rect;

    if (flag == VLGUI_WIN_DRAW_INIT) {
        vlGui_windowClear(win);
    }

    data = (ePet_windowData_t *)win->userData;

    // y = win->win_height - ePent_bmpHight[data->stage];
    // rect.x = (win->win_width - EPET_STATE_BAR_WIDTH - ePent_bmpWidth[data->stage]) >> 1;
    // rect.y = y;
    // rect.width = 0xff;
    // rect.hight = 0xff;
    // vlGui_drawBitmapInRect(win, &rect, vlGui_bmpBaby1);

    dx = win->win_width - EPET_STATE_BAR_WIDTH;
    w = win->win_width;
    vlGui_drawLine(win, dx - 2, 0, dx - 2, win->win_height, 1, VLGUI_COLOR_WHITE);

    win->win_width = EPET_STATE_BAR_WIDTH;
    vlGui_windowMove(win, dx, 0);
    vlGui_itemMenuDrawWin(win, &data->itemMenu, flag);
    vlGui_windowMove(win, -dx, 0);
    win->win_width = w;

    ePet_babyShowJump(win);
}

static int
ePet_windowProcessKeyCb(vlGui_window_t *win, uint8_t key)
{
    ePet_windowData_t *data;

    data = (ePet_windowData_t *)win->userData;

    switch (key) {
    case VLGUI_KEY_UP:
    case VLGUI_KEY_DOWN:
        vlGui_itemMenuProcessKey(win, &data->itemMenu, key);
        return 0;
    default:
        return -1;
    }
}

static void
ePet_windowAddStateItem(vlGui_itemMenuData_t *itemMenu)
{
    static vlGui_itemMenuEntry_t stateEntries[] = {
        /* Health */
        {.text = "\xA1\xBD\xB5\xBF"},
        {.text = "Health"},
        /* Hanppy */
        {.text = "\xC4\xD0\xE9\xC7"},
        {.text = "Hanppy"},
        /* Hungary */
        {.text = "\xA2\xBC\xF6\xB6"},
        {.text = "Hungary"},
        /* Education */
        {.text = "\xCC\xBD\xFD\xD3"},
        {.text = "Edu"},

    };

    for (uint8_t i = 0; i < sizeof(stateEntries) / sizeof(stateEntries[0]); i++) {
        vlGui_itemMenuAddEntry(itemMenu, &stateEntries[i]);
    }
}

static int
ePet_windowDrawEntryCb_t(vlGui_window_t *win, vlGui_windowRectArea_t *area,
                         vlGui_itemMenuEntry_t *entry)
{
    uint8_t cnt;

    area->y += 2;
    if (!memcmp(entry->text, "Health", 6)) {
        for (cnt = 0; cnt < 3; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpBlood);
            area->x += 7;
        }
        for (; cnt < 5; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpBloodEmpty);
            area->x += 7;
        }
        return 0;
    }
    if (!memcmp(entry->text, "Hanppy", 6)) {
        for (cnt = 0; cnt < 3; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpHeart);
            area->x += 7;
        }
        for (; cnt < 5; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpHeartEmpty);
            area->x += 7;
        }
        return 0;
    }
    if (!memcmp(entry->text, "Hungary", 7)) {
        for (cnt = 0; cnt < 3; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpFood);
            area->x += 7;
        }
        for (; cnt < 5; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpFoodEmpty);
            area->x += 7;
        }
        return 0;
    }
    if (!memcmp(entry->text, "Edu", 3)) {
        for (cnt = 0; cnt < 3; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpEducation);
            area->x += 8;
        }
        for (; cnt < 5; cnt++) {
            vlGui_drawBitmapInRect(win, area, vlGui_bmpEducationEmpty);
            area->x += 8;
        }
        return 0;
    }

    return -1;
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
    vlGui_engine_t *engine;

    win = vlGui_windowCreate(parent, x, y, width, height, sizeof(ePet_windowData_t));
    if (!win) {
        return NULL;
    }

    data = (ePet_windowData_t *)win->userData;

    memset(data, 0, sizeof(ePet_windowData_t));

    vlGui_itemMenuInitData(win, &data->itemMenu, vlGui_wenquan_9pt, vlGui_engineCurveInOutQuart);
    ePet_windowAddStateItem(&data->itemMenu);
    vlGui_itemMenuSetEntryDrawCb(&data->itemMenu, ePet_windowDrawEntryCb_t);

    vlGui_windowSetDrawCb(win, ePet_windowDrawCb);
    vlGui_windowSetKeyCb(win, ePet_windowProcessKeyCb);

    engine = &data->engine;
    vlGui_engineInit(engine, vlGui_engineCurveNone, NULL, data);
    vlGui_windowAppendEngine(win, engine);

    return win;
}