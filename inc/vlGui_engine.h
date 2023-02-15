/**
 * @file vlGui_engine.h
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
#ifndef _VLGUI_ENGIN_H_
#define _VLGUI_ENGIN_H_
#include <stdint.h>

typedef void (*vlGui_engineProcessCb_t)(void *param, int16_t delta);
typedef float (*vlGui_engineCurve_t)(float x);

/**
 * @brief The engine is used to implement animation and help to give a more 
 * vivid vision. It can has a variety of dynamic effects via using different
 * physical curve.
 * 
 */
struct vlGui_engine
{
    uint8_t enabled:1;
    int16_t distance;           /**< Where we want to arrive after duration */
    int16_t currentDistance;    /**< How long we have moved                 */
    uint16_t duration;          /**< The duration this engine will spend    */
    uint32_t startTime;         /**< The absolute start time                */
    vlGui_engineCurve_t curveFunc;
    vlGui_engineProcessCb_t processCb;
    struct vlGui_engine *next;
};
typedef struct vlGui_engine vlGui_engine_t;

float vlGui_engineCurveInOutQuart(float x);
float vlGui_engineCurveOutQuint(float x);
float vlGui_engineCurveOutBack(float x);
float vlGui_engineCurveOutElastic(float x);
float vlGui_engineCurveOutBounce(float x);

/**
 * @brief Initialize an engine. Can chose which curve will be used.
 * 
 * @param engine 
 * @param curve Ease curve for animation
 * @param cb Callback to do the actual animation
 */
void vlGui_engineInit(vlGui_engine_t *engine, vlGui_engineCurve_t curve,
                      vlGui_engineProcessCb_t cb);

/**
 * @brief Render all of views by the given engines. The engines can be a 
 * linked list.
 * 
 * @param engines 
 * @param param
 */
void vlGui_engineRender(vlGui_engine_t *engines, void *param);

/**
 * @brief Start an engine. Set the duration and the destination which will help
 * to implement a vivid vision.
 * 
 * @param engine 
 * @param destination 
 * @param durationMs 
 */
void vlGui_engineStart(vlGui_engine_t *engine, int16_t destination, 
                         uint16_t durationMs);

#endif // _VLGUI_ENGIN_H_