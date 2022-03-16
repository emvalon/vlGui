#include "vlonGui.h"
#include "vlonGui_input.h"
#include <string.h>

struct vlonGui_t *vlonGui_cur_screen;

int vlonGui_screen_init(struct vlonGui_t *screen, int16_t width, int16_t height)
{
    struct vlonGui_window_t *win;

    memset(screen, 0, sizeof(*screen));
    screen->fresh = 1;
    screen->width   = width;
    screen->height  = height;
    vlonGui_cur_screen = screen;

    win = vlonGui_windowCreate(NULL, 0, 0, width, height, 0);
    if(!win) {
        return -1;
    }

    vlonGui_cur_screen->window = win;
}


void vlonGui_register_driver(struct vlonGui_t *screen, struct vlonGui_driver_t *driver)
{
    screen->displayDriver = driver;
    driver->pInit();
}

static void vlonGui_refreshAllChildren(struct vlonGui_window_t *win)
{
    struct vlonGui_window_t *child;

    child = win->child;

    while (child) {
        vlonGui_windowFresh(child);
        vlonGui_refreshAllChildren(child);
        child = child->next;
    }
}

void vlonGui_refresh(void)
{
    uint8_t key;
    struct vlonGui_window_t *win;

    // 获取最上层的window 
    win = vlonGui_cur_screen->window;
    for(; win->next; win = win->next);
    //处理队列中的按键
    while(1) {
        key = vlonGui_inputGetKey();
        if(key == VLGUI_KEY_NONE) {
            break;
        }

        win->pProcessKey(win, key);
    }


    if(vlonGui_cur_screen->fresh) {
        // vlonGui_cur_screen->fresh = 0;
        vlonGui_windowFresh(win);
        vlonGui_refreshAllChildren(win);
        vlonGui_cur_screen->displayDriver->pFresh();
    }
}

struct vlonGui_window_t * vlinGui_getMainWindow(struct vlonGui_t *screen)
{
    return screen->window;
}

void vlonGui_setFont(struct vlonGui_font_t *font)
{
    vlonGui_cur_screen->curFont = font;
}

