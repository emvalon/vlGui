#ifndef _EPET_H_
#define _EPET_H_

#include "vlGui.h"
#include "widgets/vlGui_itemMenu.h"

#define EPET_STATE_BAR_WIDTH (52)

enum ePent_growthStage {
    EPENT_GROWTH_BABY = 0,
    EPENT_GROWTH_TODDLER,
    EPENT_GROWTH_TEENS,
    EPENT_GROWTH_ADULT,
    EPENT_GROWTH_MAX,
};
typedef enum ePent_growthStage ePent_growthStage_t;
struct ePet_windowData {
    uint8_t lastX;
    uint8_t lastY;
    uint8_t animation : 1;
    ePent_growthStage_t stage;
    vlGui_engine_t engine;
    vlGui_itemMenuData_t itemMenu;
};
typedef struct ePet_windowData ePet_windowData_t;

struct ePet_window {
    vlGui_window_t win;
    ePet_windowData_t data;
};
typedef struct ePet_window ePet_window_t;

vlGui_window_t *ePet_windowCreate(vlGui_window_t *parent, int16_t x, int16_t y, int16_t width,
                                  int16_t height);

/* Babay stage functions */
void ePet_babyShowJump(vlGui_window_t *win);

#endif // _EPET_H_