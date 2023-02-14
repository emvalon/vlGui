/**
 * @file vlonGui_engine.h
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-07
 * 
 * Copyright © 2021 - 2023 Weilong Shen (valonshen@foxmail.com)
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
#ifndef _VLONGUI_ENGIN_H_
#define _VLONGUI_ENGIN_H_
#include <stdint.h>

typedef void (*vlonGui_engine_cb_t)(void *param, int16_t delta);

/**
 * @brief The engine is used to implement animation and help to give a more 
 * vivid vision. It can has a variety of dynamic effects via using different
 * physical curve.
 * 
 */
struct vlonGui_engine
{
    uint8_t flags;
    int16_t destination;    /**< Where we want to arrive after duration */
    int16_t currentPoint;   /**< The current point we have moved        */
    uint16_t startTime;     /**< The absolute start time                */
    uint16_t duration;      /**< The duration this engine will spend    */
    uint16_t updatedTime;   /**< The last time this engine was updated  */   
    vlonGui_engine_cb_t processCb;
    void *param;
    struct vlonGui_engine *next;
    union
    {
        int16_t delta;
    };
    
};
typedef struct vlonGui_engine vlonGui_engine_t;

/**
 * @brief Initialize an engine. Can chose which curve will be used.
 * 
 * @param engine 
 * @param cb 
 * @param param
 * @param flag 
 */
void vlonGui_engineInit(vlonGui_engine_t *engine, vlonGui_engine_cb_t cb, 
                        void *param, uint8_t flag);

/**
 * @brief Render all of views by the given engines. The engines can be a 
 * linked list.
 * 
 * @param engines 
 */
void vlonGui_engineRender(vlonGui_engine_t *engines);

/**
 * @brief Start an engine. Set the duration and the destination which will help
 * to implement a vivid vision.
 * 
 * @param engine 
 * @param destination 
 * @param durationMs 
 */
void vlonGui_engineStart(vlonGui_engine_t *engine, int16_t destination, 
                         uint16_t durationMs);

#endif // _VLONGUI_ENGIN_H_