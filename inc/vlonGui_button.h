/**
 * @file vlonGui_button.h
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