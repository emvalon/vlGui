#ifndef _VLONGUI_BUTTON_H_
#define _VLONGUI_BUTTON_H_

#include <stdint.h>
#include "vlonGui_window.h"
#include "vlonGui_input.h"
#include "vlonGui_fonts.h"

struct vlonGui_button_t {
    struct vlonGui_window_t win;

    uint8_t pressed;
    char *text;
    struct vlonGui_font_t *font;
};

struct vlonGui_button_t * vlonGui_buttonCreate(struct vlonGui_window_t *parent, int16_t x, 
                                               int16_t y, int16_t width, uint16_t height);

void vlonGui_buttonSetText(struct vlonGui_button_t *btn, char *text);

void vloonGui_buttonSetFont(struct vlonGui_button_t *btn, struct vlonGui_font_t *font);

#endif