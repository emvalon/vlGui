/**
 * @file vlGui_port.c
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
#include "vlGui.h"
#include "vlGui_port.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

struct vlGui_driver_t vlGui_driver;
uint8_t ms_per_tick;

void * vlGui_malloc(uint32_t size)
{
    return malloc(size);
}

void vlGui_free(void *addr)
{
    return free(addr);
}

uint32_t vlGui_getTime(void)
{
    return times(NULL) * ms_per_tick;
}

void vlGui_delay(uint32_t ms)
{
    usleep(ms * 1000);
}

// static void vlGui_portDrawPixel(uint16_t x, uint16_t y, uint8_t color)
// {
//     ssd1306_DrawPixel(x, y, color);
// }

struct vlGui_driver_t * vlGui_portGetDriver(void)
{
    ms_per_tick = 1000 / sysconf(_SC_CLK_TCK);

    vlGui_driver.pInit = vlGui_portInit;
    vlGui_driver.pDrawPoint = vlGui_portDrawPixel;
    vlGui_driver.pFresh = vlGui_portRefresh;
    vlGui_driver.pGetPointColor = vlGui_portGetPixelColor;

    return &vlGui_driver;
}