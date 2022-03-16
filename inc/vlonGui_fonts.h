/**
 * @file vlonGui_fonts.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-09
 * 
 * Copyright © 2019-2021 Weilong Shen (valonshen@foxmail.com).
 * All rights reserved.
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
