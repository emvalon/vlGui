/**
 * @file vlGui_msgBox.h
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
#ifndef _VLGUI_MSGBOX_H_
#define _VLGUI_MSGBOX_H_

#include "vlGui_window.h"

typedef void (* processCb_func_t)(uint8_t ok);

struct vlGui_msgBox_t {
    vlGui_window_t win;

    char *title;
    char *text;
    processCb_func_t pProcessCb;
};

struct vlGui_msgBox_t * vlGui_msgBoxCreate(vlGui_window_t *parent);

void vlGui_msgBoxSetTitle(struct vlGui_msgBox_t *msgbox, char *str);

void vlGui_msgBoxSetText(struct vlGui_msgBox_t *msgbox, char *str);

#endif