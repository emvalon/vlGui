#ifndef _VLONGUI_SELECTOR_H_
#define _VLONGUI_SELECTOR_H_

#include <stdint.h>
#include "vlonGui_window.h"
#include "vlonGui_input.h"

struct vlonGui_selector_t {
    struct vlonGui_window_t win;

    struct vlonGui_font_t *bigFont;
    struct vlonGui_font_t *smallFont;
    void *entry;
    uint8_t select;
    uint8_t num;
};

struct vlonGui_selector_t * vlonGui_selectorCreate(struct vlonGui_window_t *parent);

int vlonGui_selectorAddEntry(struct vlonGui_selector_t *sel, char *str);

void vlonGui_selectorDelete(struct vlonGui_selector_t *sel);
#endif