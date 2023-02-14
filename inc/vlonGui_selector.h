/**
 * @file vlonGui_selector.h
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
#ifndef _VLONGUI_SELECTOR_H_
#define _VLONGUI_SELECTOR_H_

#include <stdint.h>
#include "vlonGui_window.h"
#include "vlonGui_input.h"

struct vlonGui_selector_t {
    struct vlonGui_window_t win;

    const struct vlonGui_font_t *bigFont;
    const struct vlonGui_font_t *smallFont;
    void *entry;
    uint8_t index;
    uint8_t temp_index;
    uint8_t num;
};


int vlonGui_selectorProcessKey(struct vlonGui_window_t *win, uint8_t key);

struct vlonGui_selector_t * vlonGui_selectorCreate(struct vlonGui_window_t *parent);

int vlonGui_selectorAddEntry(struct vlonGui_selector_t *sel, char *str);

void vlonGui_selectorDelete(struct vlonGui_selector_t *sel);
#endif