/**
 * @file vlGui_u8FontsLib.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-26
 *
 * Copyright 2024 - 2025 Weilong Shen (valonshen@foxmail.com)
 *
 * Copyright (c) 2016, olikraus@gmail.com
 * Modified from Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)
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
#include "fonts/vlGui_u8Fonts.h"

#define U8G2_FONT_SECTION(name)
/*
 * Tips: !! For more details about supported fonts, refer to
 * https://github.com/olikraus/u8g2/wiki/fntlistall.
 */

/*
  Fontname: open_iconic_weather_1x
  Copyright: https://github.com/iconic/open-iconic, SIL OPEN FONT LICENSE
  Glyphs: 6/6
  BBX Build Mode: 0
*/
const uint8_t u8g2_font_open_iconic_weather_1x_t[103] U8G2_FONT_SECTION(
    "u8g2_font_open_iconic_weather_1x_t") =
    "\6\0\3\3\4\4\2\2\5\10\10\0\0\10\0\10\0\0\13\0\0\0J@\13h\216\67,Y\16\7"
    "\311\1A\15\210\212\63\234J\206%\313\341 \71B\13w\212#\134\244]l\42\0C\17\210\212\67,"
    "Y\16\263YJ\266H\22\0D\12V\217%\262\220H)\0E\14f\217\21\14\311H\64Q\60\0\0"
    "\0\0\4\377\377\0";

const struct vlGui_font_t __vlGui_font_openIconicWeather8x8 = {
    .version = 0x1,
    .encoding = VLGUI_FONT_ENCOIDING_U8G2_UNICODE,
    .fontWidth = 8,
    .fontHeight = 8,
    .bytesPerChar = 0,
    .tableLen = 0,
    .data = u8g2_font_open_iconic_weather_1x_t,
};
const struct vlGui_font_t *const vlGui_font_openIconicWeather8x8 =
    &__vlGui_font_openIconicWeather8x8;

/*
  Fontname: open_iconic_embedded_1x
  Copyright: https://github.com/iconic/open-iconic, SIL OPEN FONT LICENSE
  Glyphs: 17/17
  BBX Build Mode: 0
*/
const uint8_t u8g2_font_open_iconic_embedded_1x_t[244] U8G2_FONT_SECTION(
    "u8g2_font_open_iconic_embedded_1x_t") =
    "\21\0\3\3\4\4\3\3\5\10\10\0\0\10\0\10\0\0\14\0\0\0\327@\14h,\306%\32\211\256"
    "F.\0A\15\210$\236\224\310f\71\304\201\62\0B\16\210$\236\320d\31\231&&\243\14\0C\14"
    "t.\216(D\212E&\42\0D\13v-\226\214r\70\230H\2E\13w$v\34&\234-J\1"
    "F\14X\64^XL\232P$\61\0G\16\207$^t\70\223\304$\242\312\344\60H\14\210$fX"
    "\271T\32N\305\0I\12h,\306\345\341Ar\1J\16\205%VP\42\212\214&\242\220,\4K\16"
    "\210$\26m\24\241u\213\214b$\0L\12\207$\306\357\207\312!\0M\14\205%\226dh\234\314!"
    "!\0N\20\207$^\70\30\311\24\222J#\261\330\10\0O\16\210$\26-\70\244\247C\342\20\22\0"
    "P\14\207$N%\32\7\315Q\225\0\0\0\0\4\377\377\0";

const struct vlGui_font_t __vlGui_font_openIconicEmbeded8x8 = {
    .version = 0x1,
    .encoding = VLGUI_FONT_ENCOIDING_U8G2_UNICODE,
    .fontWidth = 8,
    .fontHeight = 8,
    .bytesPerChar = 0,
    .tableLen = 0,
    .data = u8g2_font_open_iconic_embedded_1x_t,
};
const struct vlGui_font_t *const vlGui_font_openIconicEmbeded8x8 =
    &__vlGui_font_openIconicEmbeded8x8;