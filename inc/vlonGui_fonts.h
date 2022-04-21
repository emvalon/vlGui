/**
 * @file vlonGui_fonts.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-09
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
#ifndef _VLONGUI_FONTS_H_
#define _VLONGUI_FONTS_H_

#include "vlonGui.h"

struct vlonGui_font_t 
{
    const uint8_t FontWidth;    /*!< Font width in pixels */
    const uint8_t FontHeight;         /*!< Font height in pixels */
    const uint16_t *data;       /*!< Pointer to data font data array */
};

extern struct vlonGui_font_t vlonGui_font6x8;

extern struct vlonGui_font_t vlonGui_font7x10;

extern struct vlonGui_font_t vlonGui_font11x18;

extern struct vlonGui_font_t vlonGui_font16x26;

#endif
