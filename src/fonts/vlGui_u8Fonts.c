/**
 * @file vlGui_u8Fonts.c
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
#include "vlGui.h"
#include "vlGui_base.h"

#define U8G2_WITH_UNICODE 1
#define U8G2_FONT_DATA_STRUCT_SIZE (23)
#ifndef u8x8_pgm_read
#define u8x8_pgm_read(adr) (*(const uint8_t *)(adr))
#endif

struct vlGui_u8FontsInfo {
    /* offset 0 */
    uint8_t glyph_cnt;
    uint8_t bbx_mode;
    uint8_t bits_per_0;
    uint8_t bits_per_1;

    /* offset 4 */
    uint8_t bits_per_char_width;
    uint8_t bits_per_char_height;
    uint8_t bits_per_char_x;
    uint8_t bits_per_char_y;
    uint8_t bits_per_delta_x;

    /* offset 9 */
    int8_t max_char_width;
    int8_t max_char_height; /* overall height, NOT ascent. Instead ascent = max_char_height +
                               y_offset */
    int8_t x_offset;
    int8_t y_offset;

    /* offset 13 */
    int8_t ascent_A;
    int8_t descent_g; /* usually a negative value */
    int8_t ascent_para;
    int8_t descent_para;

    /* offset 17 */
    uint16_t start_pos_upper_A;
    uint16_t start_pos_lower_a;

    /* offset 21 */
#ifdef U8G2_WITH_UNICODE
    uint16_t start_pos_unicode;
#endif

    const uint8_t *font;
};
typedef struct vlGui_u8FontsInfo vlGui_u8FontsInfo_t;

/* from ucglib... */
struct vlGui_u8FontsDecode {
    const uint8_t *decode_ptr; /* pointer to the compressed data */

    uint16_t target_x;
    uint16_t target_y;

    int8_t x; /* local coordinates, (0,0) is upper left */
    int8_t y;
    int8_t glyph_width;
    int8_t glyph_height;

    uint8_t decode_bit_pos; /* bitpos inside a byte of the compressed data */
    uint8_t is_transparent;
    uint8_t fg_color;
    uint8_t bg_color;
#ifdef U8G2_WITH_FONT_ROTATION
    uint8_t dir; /* direction */
#endif
};
typedef struct vlGui_u8FontsDecode vlGui_u8FontsDecode_t;

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */
static uint16_t
vlGui_u8FontsGetWord(const uint8_t *fontData, uint8_t offset)
{
    uint16_t pos;
    fontData += offset;
    pos = u8x8_pgm_read(fontData);
    fontData++;
    pos <<= 8;
    pos += u8x8_pgm_read(fontData);
    return pos;
}

static uint8_t
vlGui_u8FontsGetByte(const uint8_t *fontData, uint8_t offset)
{
    fontData += offset;
    return u8x8_pgm_read(fontData);
}

void
vlGui_u8FontsReadInfo(vlGui_u8FontsInfo_t *fontInfo, const uint8_t *data)
{
    /* offset 0 */
    fontInfo->glyph_cnt = vlGui_u8FontsGetByte(data, 0);
    fontInfo->bbx_mode = vlGui_u8FontsGetByte(data, 1);
    fontInfo->bits_per_0 = vlGui_u8FontsGetByte(data, 2);
    fontInfo->bits_per_1 = vlGui_u8FontsGetByte(data, 3);

    /* offset 4 */
    fontInfo->bits_per_char_width = vlGui_u8FontsGetByte(data, 4);
    fontInfo->bits_per_char_height = vlGui_u8FontsGetByte(data, 5);
    fontInfo->bits_per_char_x = vlGui_u8FontsGetByte(data, 6);
    fontInfo->bits_per_char_y = vlGui_u8FontsGetByte(data, 7);
    fontInfo->bits_per_delta_x = vlGui_u8FontsGetByte(data, 8);

    /* offset 9 */
    fontInfo->max_char_width = vlGui_u8FontsGetByte(data, 9);
    fontInfo->max_char_height = vlGui_u8FontsGetByte(data, 10);
    fontInfo->x_offset = vlGui_u8FontsGetByte(data, 11);
    fontInfo->y_offset = vlGui_u8FontsGetByte(data, 12);

    /* offset 13 */
    fontInfo->ascent_A = vlGui_u8FontsGetByte(data, 13);
    fontInfo->descent_g = vlGui_u8FontsGetByte(data, 14);
    fontInfo->ascent_para = vlGui_u8FontsGetByte(data, 15);
    fontInfo->descent_para = vlGui_u8FontsGetByte(data, 16);

    /* offset 17 */
    fontInfo->start_pos_upper_A = vlGui_u8FontsGetWord(data, 17);
    fontInfo->start_pos_lower_a = vlGui_u8FontsGetWord(data, 19);

    /* offset 21 */
#ifdef U8G2_WITH_UNICODE
    fontInfo->start_pos_unicode = vlGui_u8FontsGetWord(data, 21);
#endif

    fontInfo->font = data;
}

/**
 * @brief Find the starting point of the glyph data.
 * @return const uint8_t*
 */
static const uint8_t *
vlGui_u8FontsGetGlyphData(const vlGui_u8FontsInfo_t *fontInfo, uint16_t encoding)
{
    const uint8_t *font = fontInfo->font;
    font += U8G2_FONT_DATA_STRUCT_SIZE;

    if (encoding <= 255) {
        if (encoding >= 'a') {
            font += fontInfo->start_pos_lower_a;
        } else if (encoding >= 'A') {
            font += fontInfo->start_pos_upper_A;
        }

        for (;;) {
            if (u8x8_pgm_read(font + 1) == 0) {
                break;
            }
            if (u8x8_pgm_read(font) == encoding) {
                /* skip encoding and glyph size */
                return font + 2;
            }
            font += u8x8_pgm_read(font + 1);
        }
    }
#ifdef U8G2_WITH_UNICODE
    else {
        uint16_t e;
        const uint8_t *unicode_lookup_table;

        font += fontInfo->start_pos_unicode;
        unicode_lookup_table = font;

        /* issue 596: search for the glyph start in the unicode lookup table */
        do {
            font += vlGui_u8FontsGetWord(unicode_lookup_table, 0);
            e = vlGui_u8FontsGetWord(unicode_lookup_table, 2);
            unicode_lookup_table += 4;
        } while (e < encoding);

        for (;;) {
            e = u8x8_pgm_read(font);
            e <<= 8;
            e |= u8x8_pgm_read(font + 1);

            // removed, there is now the new index table
            // #ifdef  __unix__
            //      if ( encoding < e )
            //        break;
            // #endif

            if (e == 0)
                break;

            if (e == encoding) {
                // removed, there is now the new index table
                // #ifdef  __unix__
                //	u8g2->last_font_data = font;
                //	u8g2->last_unicode = encoding;
                // #endif
                return font + 3; /* skip encoding and glyph size */
            }
            font += u8x8_pgm_read(font + 2);
        }
    }
#endif

    return NULL;
}

static uint8_t
vlGui_u8FontsDecodeGetUnsignedBits(vlGui_u8FontsDecode_t *f, uint8_t cnt)
{
    uint8_t val;
    uint8_t bit_pos = f->decode_bit_pos;
    uint8_t bit_pos_plus_cnt;

    // val = *(f->decode_ptr);
    val = u8x8_pgm_read(f->decode_ptr);

    val >>= bit_pos;
    bit_pos_plus_cnt = bit_pos;
    bit_pos_plus_cnt += cnt;
    if (bit_pos_plus_cnt >= 8) {
        uint8_t s = 8;
        s -= bit_pos;
        f->decode_ptr++;
        // val |= *(f->decode_ptr) << (8-bit_pos);
        val |= u8x8_pgm_read(f->decode_ptr) << (s);
        // bit_pos -= 8;
        bit_pos_plus_cnt -= 8;
    }
    val &= (1U << cnt) - 1;
    // bit_pos += cnt;

    f->decode_bit_pos = bit_pos_plus_cnt;
    return val;
}

static int8_t
vlGui_u8FontsDecodeGetSignedBits(vlGui_u8FontsDecode_t *f, uint8_t cnt)
{
    int8_t v, d;
    v = (int8_t)vlGui_u8FontsDecodeGetUnsignedBits(f, cnt);
    d = 1;
    cnt--;
    d <<= cnt;
    v -= d;
    return v;
    // return (int8_t)vlGui_u8FontsDecodeGetUnsignedBits(f, cnt) - ((1<<cnt)>>1);
}

static void
vlGui_u8FontsSetupDecode(const vlGui_u8FontsInfo_t *font, vlGui_u8FontsDecode_t *decode,
                         const uint8_t *glyphData)
{
    decode->decode_ptr = glyphData;
    decode->decode_bit_pos = 0;

    /* 8 Nov 2015, this is already done in the glyph data search procedure */
    /*
    decode->decode_ptr += 1;
    decode->decode_ptr += 1;
    */

    decode->glyph_width = vlGui_u8FontsDecodeGetUnsignedBits(decode, font->bits_per_char_width);
    decode->glyph_height = vlGui_u8FontsDecodeGetUnsignedBits(decode, font->bits_per_char_height);

    decode->fg_color = VLGUI_COLOR_WHITE;
    decode->bg_color = VLGUI_COLOR_BLACK;
}

static void
vlGui_u8FontDrawDecodeLine(vlGui_window_t *win, vlGui_u8FontsDecode_t *decode, int16_t posX,
                           int16_t posY, uint8_t len, uint8_t isForeground)
{
    uint8_t cnt;     /* total number of remaining pixels, which have to be drawn */
    uint8_t rem;     /* remaining pixel to the right edge of the glyph */
    uint8_t current; /* number of pixels, which need to be drawn for the draw procedure */
                     /* current is either equal to cnt or equal to rem */

    /* local coordinates of the glyph */
    uint8_t lx, ly;

    /* target position on the screen */
    uint16_t x, y;

    cnt = len;

    /* get the local position */
    lx = decode->x;
    ly = decode->y;

    for (;;) {
        /* calculate the number of pixel to the right edge of the glyph */
        rem = decode->glyph_width;
        rem -= lx;

        /* calculate how many pixel to draw. This is either to the right edge */
        /* or lesser, if not enough pixel are left */
        current = rem;
        if (cnt < rem)
            current = cnt;

        /* get target position */
        x = posX + lx;
        y = posY + ly;

        /* draw foreground and background (if required) */
        if (isForeground) {
            vlGui_drawHLine(win, x, y, current, 1, VLGUI_COLOR_WHITE);
        } else if (decode->is_transparent == 0) {
            vlGui_drawHLine(win, x, y, current, 1, VLGUI_COLOR_BLACK);
        }

        /* check, whether the end of the run length code has been reached */
        if (cnt < rem)
            break;
        cnt -= rem;
        lx = 0;
        ly++;
    }
    lx += cnt;

    decode->x = lx;
    decode->y = ly;
}

static int8_t
vlGui_u8FontsDrawUnicode(vlGui_window_t *win, const vlGui_u8FontsInfo_t *fontInfo, int16_t posX,
                         int16_t posY, const uint8_t *glyphData)
{
    uint8_t a, b;
    int8_t x, y;
    int8_t d;
    int8_t h;
    vlGui_u8FontsDecode_t _decode;
    vlGui_u8FontsDecode_t *decode = &_decode;

    vlGui_u8FontsSetupDecode(fontInfo, decode, glyphData);
    h = decode->glyph_height;

    x = vlGui_u8FontsDecodeGetSignedBits(decode, fontInfo->bits_per_char_x);
    y = vlGui_u8FontsDecodeGetSignedBits(decode, fontInfo->bits_per_char_y);
    d = vlGui_u8FontsDecodeGetSignedBits(decode, fontInfo->bits_per_delta_x);

    if (decode->glyph_width > 0) {        
        posX += x;
        posY -= h + y;

        /* reset local x/y position */
        decode->x = 0;
        decode->y = 0;

        /* decode glyph */
        for (;;) {
            a = vlGui_u8FontsDecodeGetUnsignedBits(decode, fontInfo->bits_per_0);
            b = vlGui_u8FontsDecodeGetUnsignedBits(decode, fontInfo->bits_per_1);
            do {
                vlGui_u8FontDrawDecodeLine(win, decode, posX, posY, a, 0);
                vlGui_u8FontDrawDecodeLine(win, decode, posX, posY, b, 1);
            } while (vlGui_u8FontsDecodeGetUnsignedBits(decode, 1) != 0);
            if (decode->y >= h)
                break;
        }
    }
    return d;
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
uint16_t
vlGui_u8FontsDrawGlyph(vlGui_window_t *win, int16_t x, int16_t y, uint16_t encoding)
{
    struct vlGui_t *screen;
    const uint8_t *glyphData;
    vlGui_u8FontsInfo_t fontInfo;
    uint16_t dx = 0;

    screen = vlGui_getCurScreenInstance();

    vlGui_u8FontsReadInfo(&fontInfo, screen->curFont->data);

    glyphData = vlGui_u8FontsGetGlyphData(&fontInfo, encoding);
    if (glyphData != NULL) {
        dx = vlGui_u8FontsDrawUnicode(win, &fontInfo, x, y, glyphData);
    }
    return dx;
}