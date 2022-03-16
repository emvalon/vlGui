#include "vlonGui.h"
#include "vlonGui_base.h"
#include "vlonGui_input.h"
#include "vlonGui_selector.h"
#include <string.h>

struct vlonGui_selectorEntry_t {
    uint8_t index;
    char * str;
    struct vlonGui_selectorEntry_t *next;
};

static int 
vlonGui_selectorProcessKey(struct vlonGui_window_t *win, uint8_t key)
{
    struct vlonGui_selector_t *sel;

    sel = (struct vlonGui_selector_t *)win;
    if((key == VLGUI_KEY_UP) && (sel->select < (sel->num - 1))) {
        ++sel->select;
    } else if((key == VLGUI_KEY_DOWN) && (sel->select > 0)) {
        --sel->select;
    }
}

static void 
vlonGui_drawSelector(struct vlonGui_window_t *win, void *arg)
{
    struct vlonGui_selector_t *sel;
    struct vlonGui_selectorEntry_t *entry;
    struct vlonGui_font_t *font;
    int16_t h, dh, x, y, sy;
    uint8_t select;

    sel = (struct vlonGui_selector_t *)win;

    // 每个entry所占用的高度
    h = sel->bigFont->FontHeight + 4;
    // 需要将每个entry居中，计算其y坐标的偏差
    dh = (h - sel->smallFont->FontHeight) >> 1;
    // 被选中entry的y坐标
    sy = (win->win_height - sel->bigFont->FontHeight) >> 1;

    select = sel->select;

    entry = (struct vlonGui_selectorEntry_t *)sel->entry;

    vlonGui_windowClear(win);

    vlonGui_drawRectangle(win, 0, sy - 2, win->win_width, sel->bigFont->FontHeight + 4, 1);

    while(entry) {
        if(entry->index != select) {
            font  = sel->smallFont;
            y = sy - ((entry->index - select) * h) + dh;
        } else {
            font  = sel->bigFont;
            y = sy;
        }

        vlonGui_setFont(font);
        x = (win->win_width - (font->FontWidth * strlen(entry->str))) >> 1;
        vlonGui_drawString(win, x, y, entry->str, 1);
        entry = entry->next;
    }
}

struct vlonGui_selector_t *
vlonGui_selectorCreate(struct vlonGui_window_t *parent)
{
    uint16_t w,h;
    struct vlonGui_selector_t *sel;
    VLGUI_ASSERT(parent);

    sel = (struct vlonGui_selector_t *)vlonGui_windowCreate(parent, 0, 0, 
                                parent->win_width, parent->win_height, 
                                sizeof(*sel) - sizeof(*parent));
    if(!sel) {
        return sel;
    }

    sel->win.type = VLGUI_WIN_TYPE_SELECTOR;
    sel->win.pDrawWindow = vlonGui_drawSelector;
    sel->win.pProcessKey = vlonGui_selectorProcessKey;
    sel->smallFont = &vlonGui_font7x10;
    sel->bigFont   = &vlonGui_font11x18;
    sel->select = 0;
    sel->num = 0;
    sel->entry = (void *)0;
    parent->next = (struct vlonGui_window_t *)sel;

    return sel;
}

int
vlonGui_selectorAddEntry(struct vlonGui_selector_t *sel, char *str)
{
    struct vlonGui_selectorEntry_t *entry;

    entry = vlonGui_malloc(sizeof(*entry));

    if(!entry) {
        return -1;
    }

    entry->str = str;
    entry->index = sel->num;
    ++sel->num;

    entry->next = (struct vlonGui_selectorEntry_t *)sel->entry;
    sel->entry  = (void *)entry;
}

void
vlonGui_selectorDelete(struct vlonGui_selector_t *sel)
{

}