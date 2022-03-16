/**
 * @file vlonGui_input.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-16
 * 
 * Copyright © 2019-2021 Weilong Shen (valonshen@foxmail.com).
 * All rights reserved.
 * 
 */

#include "vlonGui.h"
#include "vlonGui_input.h"

#if LVGUI_KEY_MAX_NUM < 2
    #error "LVGUI_KEY_MAX_NUM must >= 2"
#endif

/* 'out' should only be modified by Gui main task and 'in' should 
 * only be changed by input task. It doesn't matter even if one operation
 * is interrupted by an other. Therefore, don't need to enter critial
 * area in vlonGui_inputEnqueueKey() and vlonGui_inputGetKey().
 */
struct vlonGui_keyBuffer {
    uint8_t in, out;
    uint8_t buf[LVGUI_KEY_MAX_NUM];
};

static struct vlonGui_keyBuffer keyBuffer = {0};

void 
vlonGui_inputEnqueueKey(uint8_t key)
{
    uint8_t in, out;

    in = keyBuffer.in;
    out = keyBuffer.out;

    /* Check if buffer is full */
    if (in == out) {
        return;
    }

    keyBuffer.buf[in] = key;
    if(++in >= LVGUI_KEY_MAX_NUM) {
        in = 0;
    }

    keyBuffer.in = in;
}

uint8_t 
vlonGui_inputGetKey(void)
{
    uint8_t key, in, out;

    
    in = keyBuffer.in;
    out = keyBuffer.out;

    if(++out >= LVGUI_KEY_MAX_NUM) {
        out = 0;
    }

    /* Check if buffer is empty */
    if (out == in) {
        return VLGUI_KEY_NONE;
    }

    key = keyBuffer.buf[out];
    keyBuffer.out = out;

    return key;
}

void
vlonGui_inputInit(void) 
{
    keyBuffer.in  = 1;
    keyBuffer.out = 0;
}