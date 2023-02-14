/**
 * @file vlonGui_base.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-07
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
#include "vlonGui.h"
#include "vlonGui_base.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * 
 * @brief Fill block with given color.
 * 
 * @param win window
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 */
void vlonGui_drawBlock(struct vlonGui_window_t *win, int16_t x, int16_t y, 
                       int16_t width, int16_t height, uint8_t color)
{
    drawBlock_func_t func;
    int16_t actualX1,actualY1;
    int16_t actualX2,actualY2;

    func = vlonGui_cur_screen->displayDriver->pDrawBlock;
    if(!func) {
        for(int16_t l = 0; l < height; l++) {
            for(int16_t r = 0; r < width; r++) {
                vlonGui_drawPoint(win, x + r, y + l, color);
            }
        }
        return;
    }

    actualX1 = win->x1 + x + win->x_offset;
    if(actualX1 > win->x2) {
        /* Out of window */
        return;
    }

    actualY1 = win->y1 + y + win->y_offset;
    if(actualY1 > win->y2) {
        /* Out of window */
        return;
    }

    actualX2 = actualX1 + width -1;
    actualY2 = actualY1 + height -1;
    if(actualX2 < win->x1) {
        /* Out of window */
        return;
    }

    actualY1 = win->y1 + y + win->y_offset;
    if(actualY2 < win->y1) {
        /* Out of window */
        return;
    }

    /* Recalcuate posation */
    if(actualX1 < win->x1) {
        actualX1 = win->x1;
    }
    if(actualX2 > win->x2) {
        actualX2 = win->x2;
    }
    if(actualY1 < win->y1) {
        actualY1 = win->y1;
    }
    if(actualY2 > win->y2) {
        actualY2 = win->y2;
    }

    func(actualX1, actualY1, actualX2 - actualX1 + 1, actualY2 - actualY1 + 1, color);
}

void vlonGui_drawPoint(struct vlonGui_window_t *win, int16_t x, int16_t y, uint8_t color)
{
    int16_t actualX,actualY;
    vlonGui_color actualColor;
    struct vlonGui_driver_t *driver;

    driver = vlonGui_cur_screen->displayDriver;
    VLGUI_ASSERT(driver->pDrawPoint);
    VLGUI_ASSERT(driver->pGetPointColor);

    actualX = win->x1 + x + win->x_offset;
    if((actualX < win->x1) || (actualX > win->x2)) {
        /* Out of window */
        return;
    }

    actualY = win->y1 + y + win->y_offset;
    if((actualY < win->y1) || (actualY > win->y2)) {
        /* Out of window */
        return;
    }

    if (color == VLGUI_COLOR_CONVERT) {
        if (driver->pGetPointColor(actualX, actualY) == VLGUI_COLOR_BLACK) {
            actualColor = VLGUI_COLOR_WHITE;
        } else {
            actualColor = VLGUI_COLOR_BLACK;
        }
    } else {
        actualColor = color;
    }
    driver->pDrawPoint(actualX, actualY, actualColor);
}

static void drawLineByPixel(struct vlonGui_window_t *win, int16_t x1, int16_t y1, 
                            int16_t x2, int16_t y2, uint8_t color)
{
    int16_t deltaX = abs(x2 - x1);
    int16_t deltaY = abs(y2 - y1);
    int16_t signX = ((x1 < x2) ? 1 : -1);
    int16_t signY = ((y1 < y2) ? 1 : -1);
    int16_t error = deltaX - deltaY;
    int16_t error2;
    
    vlonGui_drawPoint(win, x2, y2, color);
    while((x1 != x2) || (y1 != y2)) {
        vlonGui_drawPoint(win, x1, y1, color);
        error2 = error * 2;
        if(error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        } else {
            /*nothing to do*/
        }
        
        if(error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        } else {
            /*nothing to do*/
        }
    }
}

void vlonGui_drawLine(struct vlonGui_window_t *win, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, int16_t width, uint8_t color)
{
    struct vlonGui_driver_t *display;
    int16_t actualX1,actualY1;
    int16_t actualX2,actualY2;
    int16_t min, max;

    VLGUI_ASSERT(width);
    display = vlonGui_cur_screen->displayDriver;

    /* TODO: check if this line is in given window */

    if((x1 == x2) && display->pDrawLineV) {
        actualX1 = win->x1 + x1 + win->x_offset;
        actualY1 = win->y1 + y1 + win->y_offset;

        actualX2 = win->x1 + x2 + win->x_offset;
        actualY2 = win->y1 + y2 + win->y_offset;

        if((actualX1 < win->x1) || (actualX1 > win->x2)) {
            return;
        }

        min = VLGUI_MIN(actualY1, actualY2);
        max = VLGUI_MAX(actualY1, actualY2);

        if((min > win->y2) || (max < win->y1)) {
            /* Out of window */
            return;
        }

        if(min < win->y1) {
            min = win->y1;
        }

        if(max > win->y2) {
            max = win->y2;
        }

        display->pDrawLineV(actualX1, min, actualX1 , max, color);
        if(width > 1) {
            vlonGui_drawLine(win, x1 + 1, y1, x2 + 1, y2, width - 1, color);
        }
        return;
        
    }
    else if((y1 == y2) && display->pDrawLineH) {
        actualX1 = win->x1 + x1 + win->x_offset;
        actualY1 = win->y1 + y1 + win->y_offset;

        actualX2 = win->x1 + x2 + win->x_offset;
        actualY2 = win->y1 + y2 + win->y_offset;
        
        if((actualY1 < win->y1) || (actualY1 > win->y2)) {
            return;
        }

        min = VLGUI_MIN(actualX1, actualX2);
        max = VLGUI_MAX(actualX1, actualX2);

        if((min > win->x2) || (max < win->x1)) {
            /* Out of window */
            return;
        }

        if(min < win->x1) {
            min = win->x1;
        }

        if(max > win->x2) {
            max = win->x2;
        }

        display->pDrawLineH(min, actualY1, max, actualY2, color);
        if(width > 1) {
            vlonGui_drawLine(win, x1, y1 + 1, x2, y2 + 1, width - 1, color);
        }
        return;
    }

    drawLineByPixel(win, x1, y1, x2, y2, color);
    
    if(width <= 1) {
        return;
    }

    if(x1 == x2) {
        vlonGui_drawLine(win, x1 + 1, y1, x2 + 1, y2, width - 1, color);
    } else if(y2 == y1) {
        vlonGui_drawLine(win, x1, y1 + 1, x2, y2 + 1, width - 1, color);
    }
}

void vlonGui_drawBitmap(struct vlonGui_window_t *win, int16_t x, int16_t y, 
                        int16_t width, int16_t height, const uint8_t *bitmap)
{
    uint16_t l,r;
    uint8_t v;

    //宽度按8的倍数向上取整
    width = ((width + 7) / 8) * 8;

    v = *bitmap;
    for(l = 0; l < height; l++) {
        r = 0;
        while (1) {
            if(v & 0x01) {
                vlonGui_drawPoint(win, x + r, y + l, 1);
            }

            ++r;
            if((r % 8) == 0) {
                ++bitmap;
                v = *bitmap;
                if(r == width) {
                    break;
                }
            } else {
                v >>= 1;
            }
        }
    }
}

void vlonGui_drawRectangle(struct vlonGui_window_t *win, int16_t x, int16_t y, 
                          int16_t width, int16_t height, uint8_t color)
{
    int16_t x2, y2;

    x2 = x + width - 1;
    y2 = y + height -1;
    vlonGui_drawLine(win, x, y, x, y2, 1,color);
    vlonGui_drawLine(win, x2, y, x2, y2, 1, color);
    vlonGui_drawLine(win, x, y, x2, y, 1, color);
    vlonGui_drawLine(win, x, y2, x2 ,y2, 1, color);
}

// static char vlonGui_drawChar(struct vlonGui_window_t *win, int16_t x, int16_t y, char ch, uint8_t color) 
// {
//     uint32_t i, b, j;
//     struct vlonGui_font_t *font;
    
//     // Check if character is valid
//     if (ch < 32 || ch > 126) {
//         return 0;
//     }
    
//     font = vlonGui_cur_screen->curFont;
    
//     // Use the font to write
//     for(i = 0; i < font->fontHeight; i++)
//     {
//         b = font->data[(ch - 32) * font->fontHeight + i];
//         for(j = 0; j < font->fontWidth; j++)
//         {
//             if((b << j) & 0x8000)
//             {
//                 vlonGui_drawPoint(win, x + j, y + i, color);
//             }
//         }
//     }
    
//     // Return written char for validation
//     return ch;
// }

static void
vlonGui_drawGlyph(struct vlonGui_window_t *win, int16_t x, int16_t y,  
                  uint16_t glyph, const struct vlonGui_font_t *font, vlonGui_color color)
{
    int16_t dx;
    uint8_t pos;
    uint8_t width;
    uint8_t *charAddr;
    uint16_t segIndex;
    const struct vlonGui_font_table_item_t *item;

    /* Search for correct segment */
    for (segIndex = 0; segIndex < font->tableLen; segIndex++) {
        item = &font->table[segIndex];
        if (glyph <= item->end) {
            /* If doesn't contain this glyph, return*/
            if (item->start > glyph) {
                return;
            }
            break;
        }
    }
    
    /* If doesn't contain this glyph, return */
    if (segIndex >= font->tableLen) {
        return;
    }
        
    charAddr = (void *)font + item->offset;
    charAddr += font->bytesPerChar * (glyph - item->start);

    /* Draw this character */
    dx = 0;
    
    width = *charAddr;
    ++charAddr;
    pos = 7;

    for (uint8_t col = 0; col < font->fontHeight; col++)
    for (uint8_t dx = 0; dx < width; dx++) {
        if (*charAddr & (1 << pos)) {
            vlonGui_drawPoint(win, x + dx, y + col, color);
        }

        if (pos > 0) {
            --pos;
        } else {
            pos = 7;
            ++charAddr;
        }
    }    
}

static uint16_t
vlonGui_getGlyphCode(char **str, uint8_t coding)
{
    char *addr;
    uint16_t glyph;

    addr = *str;
    if (coding == VLGUI_FONT_ENCODING_GB2312) {
        if (addr[0] && addr[1]) {
#if VLGUI_UNICODE_BIGEND
            /* Get this glyph with big-endian */
            glyph = (addr[0] << 8) | (uint8_t)addr[1];
#else   
            /* Get this glyph with little-endian */
            glyph = (addr[1] << 8) | (uint8_t)addr[0];
#endif // VLGUI_UNICODE_BIGEND

            *str = *str + 2;
        } else {
            glyph = 0;
        }
    } else if (coding == VLGUI_FONT_ENCOIDING_UTF8) {
        glyph = addr[0];
        *str = *str + 1;
    } else {
        glyph = 0;
    }

    return glyph;
}

void vlonGui_drawString(struct vlonGui_window_t *win, int16_t x, 
                        int16_t y, char *str, uint8_t color)
{
    int16_t offset;
    uint16_t glyph;
    const struct vlonGui_font_t *font;

    offset = 0;
    font = vlonGui_cur_screen->curFont;
    while(1) {
        glyph = vlonGui_getGlyphCode(&str, font->encoding);
        if (!glyph) {
            break;
        }
        vlonGui_drawGlyph(win, x + offset, y, glyph, font, color);
        offset += font->fontWidth;
    }
}


static void 
vlonGui_drawCircleSection(struct vlonGui_window_t *win, int16_t x, int16_t y, 
                          int16_t x0, int16_t y0)
{
    /* upper right */
    vlonGui_drawPoint(win, x0 + x, y0 - y, VLGUI_COLOR_WHITE);
    vlonGui_drawPoint(win, x0 + y, y0 - x, VLGUI_COLOR_WHITE);
    
    /* upper left */
    vlonGui_drawPoint(win, x0 - x, y0 - y, VLGUI_COLOR_WHITE);
    vlonGui_drawPoint(win, x0 - y, y0 - x, VLGUI_COLOR_WHITE);
    
    /* lower right */
    vlonGui_drawPoint(win, x0 + x, y0 + y, VLGUI_COLOR_WHITE);
    vlonGui_drawPoint(win, x0 + y, y0 + x, VLGUI_COLOR_WHITE);
    
    /* lower left */
    vlonGui_drawPoint(win, x0 - x, y0 + y, VLGUI_COLOR_WHITE);
    vlonGui_drawPoint(win, x0 - y, y0 + x, VLGUI_COLOR_WHITE);
}

void 
vlonGui_drawCircle(struct vlonGui_window_t *win, int16_t x0, int16_t y0, 
                   uint8_t rad)
{
    int16_t f;
    int16_t ddF_x;
    int16_t ddF_y;
    int16_t x;
    int16_t y;

    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;

    vlonGui_drawCircleSection(win, x, y, x0, y0);
    
    while ( x < y )
    {
        if (f >= 0) 
        {
          y--;
          ddF_y += 2;
          f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        vlonGui_drawCircleSection(win, x, y, x0, y0);
    }
}