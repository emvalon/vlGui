/**
 * @file vlGui_freeRTOS.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-13
 * 
 * Copyright © 2021 - 2024 Weilong Shen (valonshen@foxmail.com)
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
#include <stdint.h>

void *
vlGui_osMalloc(uint32_t size)
{
    return malloc(size);
}

void
vlGui_osFree(void * addr)
{
    free(addr);
}

uint32_t
vlGui_osGetTimeMs(void)
{
    //!TODO
    return 0;
}

void
vlGui_osDelay(uint32_t ms)
{
    //!TODO
}

void
vlGui_osInit(void)
{
    
}