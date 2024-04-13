/**
 * @file vlGui_port.h
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
#ifndef _VLGUI_PORT_H_
#define _VLGUI_PORT_H_

#include "vlGui_config.h"

void * vlGui_malloc(uint32_t size);

void vlGui_free(void *addr);

uint32_t vlGui_getTimeMs(void);

void vlGui_delay(uint32_t ms);

void *vlGui_protSemphrCreate(void);

void vlGui_protSemphrDestroy(void *semphr);

void vlGui_portSemphrTake(void *semphr, uint32_t delay_time);

void vlGui_portSemphrGive(void *semphr);

void vlGui_portInit(void);

void vlGui_portDrawPixel(uint16_t x, uint16_t y, vlGui_color color);

vlGui_color vlGui_portGetPixelColor(uint16_t x, uint16_t y);

void vlGui_portRefresh(void);

/* The list of display drivers which we have supported now. 
 * Can be used as the second parameter of `vlGui_register_driver()`.
 */
extern struct vlGui_driver_t const vlGui_driverGtk;
extern struct vlGui_driver_t vlGui_driverSsd1306;

#endif 