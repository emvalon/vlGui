#include <stdio.h>
#include "games.h"
#include "games_glatSnake.h"
#include "vlonGui_selector.h"

struct vlonGui_selector_t *sel;

static int 
gameSelectorKeyCb(struct vlonGui_window_t *win, uint8_t key)
{
    if (key == VLGUI_KEY_OK) {
        switch (sel->index) {
        case 0:
            games_gultSnakeCreate(win);
            break;
        default:
            break;
        }
    } else {
        vlonGui_selectorProcessKey(win, key);
    }
}

void 
games_createSelector(struct vlonGui_window_t *win) 
{
    sel = vlonGui_selectorCreate(win);

    vlonGui_selectorAddEntry(sel, "GlutSnake");
    vlonGui_selectorAddEntry(sel, "Shen Weilong");
    vlonGui_selectorAddEntry(sel, "Valon Shen");
    vlonGui_selectorAddEntry(sel, "Ma Suhong");
    vlonGui_selectorAddEntry(sel, "Suhon Ma");
    vlonGui_selectorAddEntry(sel, "VlonGui"); 

    vlonGui_windowSetKeyCb((struct vlonGui_window_t *)sel, gameSelectorKeyCb);
}