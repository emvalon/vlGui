/**
 * @file vlGui_u8Fonts.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-26
 *
 * Copyright 2024 - 2025 Weilong Shen (valonshen@foxmail.com)
 *
 * Copyright (c) 2016, olikraus@gmail.com
 * Modified from Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)
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
#ifndef _VLGUI_U8FONTS_H_
#define _VLGUI_U8FONTS_H_

#include "vlGui.h"
#include "vlGui_fonts.h"

uint16_t vlGui_u8FontsDrawGlyph(vlGui_window_t *win, int16_t x, int16_t y, uint16_t encoding);


extern const struct vlGui_font_t *const vlGui_font_openIconicWeather8x8;

extern const struct vlGui_font_t *const vlGui_font_openIconicEmbeded8x8;
#endif
