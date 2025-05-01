/**
 * @file ePet_babyStage.c
 * @author Weilong Shen (valonshen@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2025-05-03
 *
 * Copyright 2024 - 2025 Weilong Shen (valonshen@foxmail.com)
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

#include "ePet.h"

#include "pictures/ePet/inc/vlGui_bmpBaby1.h"
#include "pictures/ePet/inc/vlGui_bmpBaby1_gloomy.h"

#define EPET_BABY_IMG_WIDTH (15)
#define EPET_BABY_IMG_HIGHT (15)

#define EPET_BABY_JUMP_HIGHT (30)

/*
 **************************************************************************************************
 * Static Functions
 **************************************************************************************************
 */
static void
ePet_babyJumpCb(void *param, int16_t delta)
{
    vlGui_windowRectArea_t rect;
    vlGui_window_t *win;
    vlGui_engine_t *engine;
    ePet_windowData_t *ePetData;
    const unsigned char *img;

    win = (vlGui_window_t *)param;
    ePetData = (ePet_windowData_t *)win->userData;
    engine = &ePetData->engine;
    vlGui_drawBlock(win, ePetData->lastX, ePetData->lastY, EPET_BABY_IMG_WIDTH, EPET_BABY_IMG_HIGHT,
                    VLGUI_COLOR_BLACK);

    rect.x = (win->win_width - EPET_STATE_BAR_WIDTH - EPET_BABY_IMG_WIDTH) >> 1;
    if (delta >= 0) {
        rect.y = win->win_height - EPET_BABY_IMG_HIGHT - delta;
    } else {
        rect.y = win->win_height - EPET_BABY_IMG_HIGHT - EPET_BABY_JUMP_HIGHT - delta;
    }
    if (rect.y > win->win_height - EPET_BABY_IMG_HIGHT - (EPET_BABY_JUMP_HIGHT >> 2)) {
        img = vlGui_bmpBaby1_gloomy;
    } else {
        img = vlGui_bmpBaby1;
    }
    rect.width = 0xff;
    rect.hight = 0xff;
    vlGui_drawBitmapInRect(win, &rect, img);
    ePetData->lastY = rect.y;

    if (!vlGui_engineIsRunning(engine)) {
        if (delta > 0) {
            vlGui_engineStart(engine, -EPET_BABY_JUMP_HIGHT, 800);
        } else {
            ePetData->animation = 0;
        }
    }
}

/*
 **************************************************************************************************
 * Public Functions
 **************************************************************************************************
 */
void
ePet_babyShowJump(vlGui_window_t *win)
{
    vlGui_engine_t *engine;
    ePet_windowData_t *ePetData;

    ePetData = (ePet_windowData_t *)win->userData;
    engine = &ePetData->engine;
    if (!ePetData->animation) {
        ePetData->animation = 1;
        ePetData->lastX = (win->win_width - EPET_STATE_BAR_WIDTH - EPET_BABY_IMG_WIDTH) >> 1;
        ePetData->lastY = win->win_height - EPET_BABY_IMG_HIGHT;

        vlGui_engineInit(engine, vlGui_engineCurveOutBack, ePet_babyJumpCb, win);
        vlGui_engineSetCumulative(engine, 1);
        vlGui_engineStart(engine, EPET_BABY_JUMP_HIGHT, 800);
    }
}