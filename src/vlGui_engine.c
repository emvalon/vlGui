/**
 * @file vlGui_engine.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2023-02-19
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
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "vlGui.h"
#include "vlGui_engine.h"

float
vlGui_engineCurveNone(float x)
{
    VLGUI_UNUSED(x);
    return 1;
}

float
vlGui_engineCurveOutBack(float x)
{
    const float c1 = 1.70158;
    const float c3 = c1 + 1;

    return 1 + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
}

float
vlGui_engineCurveInOutQuart(float x)
{
    return x < 0.5 ? 8 * x * x * x * x : 1 - powf(-2 * x + 2, 4) / 2;
}

float
vlGui_engineCurveOutQuint(float x)
{
    return 1 - powf(1 - x, 5);
}

float
vlGui_engineCurveOutElastic(float x)
{
    const float c4 = (2 * 3.1415926) / 3;

    return x == 0 ? 0 : x == 1 ? 1 : powf(2, -10 * x) * sinf((x * 10 - 0.75) * c4) + 1;
}

float
vlGui_engineCurveOutBounce(float x)
{
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        x -= 1.5 / d1;
        return n1 * x * x + 0.75;
    } else if (x < 2.5 / d1) {
        x -= 2.25 / d1;
        return n1 * x * x + 0.9375;
    } else {
        x -= 2.625 / d1;
        return n1 * x * x + 0.984375;
    }
}

static int16_t
vlGui_engineGetResult(vlGui_engine_t *engine)
{
    int16_t delta;
    int16_t expected;
    uint32_t now;
    float x;
    float result;

    if (!engine->enabled) {
        return 0;
    }

    now = vlGui_getTime();

    if (now >= (engine->startTime + engine->duration)) {
        engine->enabled = 0;
        if (engine->cumulative) {
            return engine->currentDistance + engine->distance;
        } else {
            return engine->distance - engine->currentDistance;
        }
    }

    x = (now - engine->startTime) / (float)engine->duration;
    result = engine->curveFunc(x) * engine->distance;
    if (result >= 0) {
        expected = (int16_t)(result + 0.5);
    } else {
        expected = (int16_t)(result - 0.5);
    }

    if (engine->cumulative) {
        return engine->currentDistance + expected;
    }

    if (expected != engine->currentDistance) {
        delta = expected - engine->currentDistance;
        engine->currentDistance = expected;
    } else {
        delta = 0;
    }

    return delta;
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
int16_t
vlGui_engineMap2OtherDist(vlGui_engine_t *engine, int16_t delta, int16_t newDist)
{
    float result;
    int16_t dis;

    VLGUI_ASSERT(engine->distance);

    if (engine->distance > 0) {
        dis = engine->distance;
    } else {
        dis = -engine->distance;
    }

    if (newDist < 0) {
        newDist = -newDist;
    }

    result = (delta / (float)dis) * newDist;
    return (int16_t)result;
}

void
vlGui_engineInit(vlGui_engine_t *engine, vlGui_engineCurve_t curve, vlGui_engineProcessCb_t cb,
                 void *param)
{
    memset(engine, 0, sizeof(vlGui_engine_t));
    engine->curveFunc = curve;
    engine->processCb = cb;
    engine->param = param;
}

void
vlGui_engineSetParam(vlGui_engine_t *engine, void *param)
{
    engine->param = param;
}

void
vlGui_engineSetCumulative(vlGui_engine_t *engine, bool cumulative)
{
    engine->cumulative = cumulative;
}

int
vlGui_engineSetCurve(vlGui_engine_t *engine, vlGui_engineCurve_t curve)
{
    if (vlGui_engineIsRunning(engine)) {
        return -1;
    } else {
        engine->curveFunc = curve;
        return 0;
    }
}

void
vlGui_engineAppend(vlGui_engine_t *prev, vlGui_engine_t *next)
{
    prev->next = next;
}

void
vlGui_engineRender(vlGui_engine_t *engines)
{
    for (; engines; engines = engines->next) {
        if (!engines->enabled) {
            continue;
        }
        engines->processCb(engines->param, vlGui_engineGetResult(engines));
    }
}

bool
vlGui_engineIsRunning(vlGui_engine_t *engine)
{
    return engine->enabled;
}

void
vlGui_engineStart(vlGui_engine_t *engine, int16_t distance, uint16_t durationMs)
{
    int16_t cur;

    if (engine->enabled) {
        if (engine->cumulative) {
            cur = vlGui_engineGetResult(engine);
            engine->distance -= cur - engine->currentDistance;
            engine->distance += distance;
            engine->currentDistance = cur;
        } else {
            engine->distance -= engine->currentDistance;
            engine->distance += distance;
            engine->currentDistance = 0;
        }
    } else {
        engine->distance = distance;
        engine->currentDistance = 0;
    }
    engine->enabled = 1;
    engine->duration = durationMs;
    engine->startTime = vlGui_getTime();
}

void
vlGui_engineFinish(vlGui_engine_t *engine)
{
    if (!engine->enabled) {
        return;
    }

    engine->enabled = 0;
    if (engine->cumulative) {
        engine->processCb(engine->param, engine->distance + engine->currentDistance);
    } else {
        engine->processCb(engine->param, engine->distance - engine->currentDistance);
    }
}

void
vlGui_engineStop(vlGui_engine_t *engine)
{
    engine->enabled = 0;
}