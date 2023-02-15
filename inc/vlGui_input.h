/**
 * @file vlGui_input.h
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
#ifndef _VLGUI_INPUT_H_
#define _VLGUI_INPUT_H_

#include <stdint.h>



#define VLGUI_KEY_ESC               (0xf0)

#define VLGUI_KEY_OK                (0xf9)
#define VLGUI_KEY_CANCEL            (0xfa)
#define VLGUI_KEY_LETF              (0xfb)
#define VLGUI_KEY_RIGHT             (0xfc)
#define VLGUI_KEY_UP                (0xfd)
#define VLGUI_KEY_DOWN              (0xfe)
#define VLGUI_KEY_NONE              (0xff)


void vlGui_inputEnqueueKey(uint8_t key);

uint8_t vlGui_inputGetKey(void);

void vlGui_inputInit(void);

#endif