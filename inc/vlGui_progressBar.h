/**
 * @file vlGui_progressBar.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-22
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
#ifndef _VLGUI_PROGRESSBAR_H_
#define _VLGUI_PROGRESSBAR_H_

#include <stdint.h>
#include "vlGui_window.h"
#include "vlGui_input.h"


struct vlGui_progressBar_t {
    struct vlGui_window_t win;
    uint8_t value;
    char *title;
};

struct vlGui_progressBar_t * vlGui_progressBarCreate(struct vlGui_window_t *parent, 
                                    int16_t x, int16_t y, int16_t width, uint16_t height);

void vlGui_progressBarSetValue(struct vlGui_progressBar_t * pbar, uint8_t value);

void vlGui_progressBarSetTitle(struct vlGui_progressBar_t * pbar, char *title);

#endif // _VLGUI_PROGRESSBAR_H_