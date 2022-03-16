#ifndef _VLONGUI_BASE_H_
#define _VLONGUI_BASE_H_

#include <stdint.h>
#include "vlonGui_window.h"


void vlonGui_drawBlock(struct vlonGui_window_t *win, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t color);

void vlonGui_drawPoint(struct vlonGui_window_t *win, int16_t x, int16_t y, uint8_t color);

void vlonGui_drawLine(struct vlonGui_window_t *win, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t width, uint8_t color);

void vlonGui_drawBitmap(struct vlonGui_window_t *win, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t *bitmap);

void vlonGui_drawRectangle(struct vlonGui_window_t *win, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t color);

void vlonGui_drawString(struct vlonGui_window_t *win, int16_t x, int16_t y, char *str, uint8_t color);
#endif