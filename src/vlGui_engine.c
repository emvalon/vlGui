#include <string.h>
#include <stdio.h>
#include <math.h>
#include "vlGui.h"
#include "vlGui_engine.h"

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

    return x == 0
           ? 0
           : x == 1
           ? 1
           : powf(2, -10 * x) * sinf((x * 10 - 0.75) * c4) + 1;
}

float
vlGui_engineCurveOutBounce(float x)
{
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        return n1 * (x -= 1.5 / d1) * x + 0.75;
    } else if (x < 2.5 / d1) {
        return n1 * (x -= 2.25 / d1) * x + 0.9375;
    } else {
        return n1 * (x -= 2.625 / d1) * x + 0.984375;
    }
}

void
vlGui_engineInit(vlGui_engine_t *engine, vlGui_engineCurve_t curve,
                 vlGui_engineProcessCb_t cb)
{
    memset(engine, 0, sizeof(vlGui_engine_t));
    engine->curveFunc = curve;
    engine->processCb = cb;
}

int16_t 
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

    if(now >= (engine->startTime + engine->duration)) {
        engine->enabled = 0;
        return engine->distance - engine->currentDistance;
    }

    x = (now - engine->startTime) / (float)engine->duration;
    result = engine->curveFunc(x);
    expected = (int16_t)(result * engine->distance);

    if (expected != engine->currentDistance) {
        delta = expected - engine->currentDistance;
        engine->currentDistance = expected;
    } else {
        delta = 0;

    }

    return delta;
}

void
vlGui_engineRender(vlGui_engine_t *engines, void *param)
{
    for (; engines; engines = engines->next) {
        if (!engines->enabled) {
            continue;
        }
        engines->processCb(param, vlGui_engineGetResult(engines));
    }
}   

void
vlGui_engineStart(vlGui_engine_t *engine, int16_t distance, 
                  uint16_t durationMs)
{
    if (engine->enabled) {
        engine->distance -= engine->currentDistance;
        engine->distance += distance;
    } else {
        engine->enabled = 1;
        engine->distance = distance;
    }
    engine->currentDistance = 0;
    engine->duration = durationMs;
    engine->startTime = vlGui_getTime();
}