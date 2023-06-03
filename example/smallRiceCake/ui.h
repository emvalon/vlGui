/**
 * @file ui.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-09
 * 
 * Copyright 2021 - 2023 Weilong Shen (valonshen@foxmail.com)
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
#ifndef _UI_H_
#define _UI_H_
#include "vlGui_window.h"

void ui_homeWinCreate(vlGui_window_t *parent, int16_t x, int16_t y, 
                      int16_t width, int16_t height);
void ui_menuWinCreate(vlGui_window_t *parent, int16_t x, int16_t y, 
                      int16_t width, int16_t height);

#endif // _UI_H_