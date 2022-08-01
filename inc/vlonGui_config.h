/**
 * @file vlonGui_config.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-08
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
#ifndef _VLONGUI_CONFIG_H_
#define _VLONGUI_CONFIG_H_

#include <stdint.h>

/*
 * Config Macro
 */

/* Enable animation feature for window */
#define LVGUI_ANIMATION                 (1)

/* The number of key can be enqueued */
#define LVGUI_KEY_MAX_NUM               (10)

/* The size of bokeh mask */
#define VLGUI_BOKEH_SIZE                (1)

/* Indicates the unicode is formed in big-endian */
#define VLGUI_UNICODE_BIGEND            (1)

/* 
 * Declaration 
 */ 
#ifndef NULL
#define NULL    ((void *)0)
#endif

typedef uint8_t     vlonGui_color;

#endif // _VLONGUI_CONFIG_H_