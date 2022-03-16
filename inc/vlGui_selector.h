/**
 * @file vlGui_selector.h
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
#ifndef _VLGUI_SELECTOR_H_
#define _VLGUI_SELECTOR_H_

#include <stdint.h>
#include "vlGui_window.h"
#include "vlGui_input.h"

struct vlGui_selector_t {
    vlGui_window_t win;

    const struct vlGui_font_t *bigFont;
    const struct vlGui_font_t *smallFont;
    void *entry;
    uint8_t index;
    uint8_t temp_index;
    uint8_t num;
};


int vlGui_selectorProcessKey(vlGui_window_t *win, uint8_t key);

struct vlGui_selector_t * vlGui_selectorCreate(vlGui_window_t *parent);

int vlGui_selectorAddEntry(struct vlGui_selector_t *sel, char *str);

void vlGui_selectorDelete(struct vlGui_selector_t *sel);
#endif