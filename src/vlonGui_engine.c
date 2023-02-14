#include <string.h>
#include "vlonGui.h"
#include "vlonGui_engine.h"

#define VLONGUI_ENGINE_RUN          (0x0001)

void
vlonGui_engineInit(vlonGui_engine_t *engine, vlonGui_engine_cb_t cb, 
                   void *param, uint8_t flag)
{
    memset(engine, 0, sizeof(vlonGui_engine_t));
    engine->flags = flag;
    engine->processCb = cb;
    engine->param = param;
}

int16_t 
vlonGui_engineGetResult(vlonGui_engine_t *engine)
{
    int16_t delta;
    uint16_t dt;
    uint16_t now;

    if (!(engine->flags & VLONGUI_ENGINE_RUN)) {
        return 0;
    }

    now = vlonGui_getTime();


    if(now >= (engine->startTime + engine->duration)) {
        engine->flags &= ~VLONGUI_ENGINE_RUN;
        return engine->destination - engine->currentPoint;
    }

    dt = now - engine->updatedTime;
    delta = (dt * engine->delta) >> 8;
    if (delta != 0) {
        engine->currentPoint += delta;
        engine->updatedTime = now;
    }

    return delta;
}

void
vlonGui_engineRender(vlonGui_engine_t *engines)
{
    for (; engines; engines = engines->next) {
        if (!(engines->flags & VLONGUI_ENGINE_RUN)) {
            continue;
        }
        engines->processCb(engines->param, vlonGui_engineGetResult(engines));
    }
}   

void
vlonGui_engineStart(vlonGui_engine_t *engine, int16_t destination, 
                    uint16_t durationMs)
{
    if (engine->flags & VLONGUI_ENGINE_RUN) {
        engine->destination -= engine->currentPoint;
        engine->destination += destination;
    } else {
        engine->flags |= VLONGUI_ENGINE_RUN;
        engine->destination = destination;
    }
    engine->currentPoint = 0;
    engine->duration = durationMs;
    engine->startTime = vlonGui_getTime();
    engine->updatedTime = engine->startTime;

    engine->delta = (destination << 8) / durationMs;
}