/**
 * @file vlGui_fonts.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-14
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
#ifndef _VLGUI_FONTS_H_
#define _VLGUI_FONTS_H_

#include "vlGui.h"

#define VLGUI_FONT_ENCOIDING_UTF8           (0)
#define VLGUI_FONT_ENCODING_GB2312          (1)

#define VLGUI_FONT_STR_DEF(name, language)                      \
    extern const unsigned char str_##name##_##language[];       \
    const unsigned char *str_##name = str_##name##_##language;  \
    const unsigned char str_##name##_##language[]

struct vlGui_font_table_item_t
{
    uint16_t start;
    uint16_t end;
    uint32_t offset;
};

struct vlGui_font_t
{
    uint8_t version;
    uint8_t encoding;
    uint8_t fontWidth;
    uint8_t fontHeight;

    uint8_t bytesPerChar;
    uint8_t resv0;
    uint16_t tableLen;

    struct vlGui_font_table_item_t table[0];
};

extern struct vlGui_font_t vlGui_font6x8;

extern struct vlGui_font_t vlGui_font7x10;

extern struct vlGui_font_t vlGui_font11x18;

extern struct vlGui_font_t vlGui_font16x26;

extern const struct vlGui_font_t * const vlGui_wenquan_9pt;
extern const struct vlGui_font_t * const vlGui_asc6x8;
extern const struct vlGui_font_t * const vlGui_asc7x10;
extern const struct vlGui_font_t * const vlGui_asc11x18;
extern const struct vlGui_font_t * const vlGui_asc16x26;
#endif
