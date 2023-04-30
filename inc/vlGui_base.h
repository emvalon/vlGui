/**
 * @file vlGui_base.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-21
 * 
 * Copyright © 2021 - 2022 Weilong Shen (valonshen@foxmail.com)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */
#ifndef _VLGUI_BASE_H_
#define _VLGUI_BASE_H_

#include <stdint.h>
#include "vlGui_window.h"


void vlGui_drawBlock(vlGui_window_t *win, int16_t x, int16_t y, int16_t width, 
                     int16_t height, uint8_t color);

void vlGui_drawPoint(vlGui_window_t *win, int16_t x, int16_t y, uint8_t color);

void vlGui_drawLine(vlGui_window_t *win, int16_t x1, int16_t y1, int16_t x2, 
                    int16_t y2, int16_t width, uint8_t color);

void vlGui_drawBitmap(vlGui_window_t *win, int16_t x, int16_t y, int16_t width, 
                      int16_t height, const uint8_t *bitmap);

void vlGui_drawRectangle(vlGui_window_t *win, int16_t x, int16_t y, 
                         int16_t width, int16_t height, uint8_t color);

void vlGui_drawString(vlGui_window_t *win, int16_t x, int16_t y, char *str, 
                      uint8_t color);

void vlGui_drawCircle(vlGui_window_t *win, int16_t x0, int16_t y0, uint8_t rad);

void vlGui_drawFilledCircle(vlGui_window_t *win, int16_t x0, int16_t y0, 
                            uint8_t rad, vlGui_color color);
#endif