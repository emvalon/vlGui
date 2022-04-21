/**
 * @file games_glatSnake.h
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
#ifndef _GAMES_GLUTSNAKE_H_
#define _GAMES_GLUTSNAKE_H_

#include "vlonGui.h"


#include<stdbool.h>
#include<stdlib.h>


#define GAMES_SCREEN_WIDTH        30
#define GAMES_SCREEN_HEIGTH       20

// Snake structure
struct game_Snake_t {
	int lenght;
	// The snake moving direction in X and Y, so just one value can be 1 or -1
	int x_dir;
	int y_dir;
	int tailPos[GAMES_SCREEN_WIDTH * GAMES_SCREEN_HEIGTH][2]; 
};

/* The whole game, that only contains 
	the player and the apple */
struct game_glutSnake_t {
	struct game_Snake_t player;
	int apple_pos[2];
    uint16_t score;
    uint8_t level;
};

int games_gultSnakeCreate(struct vlonGui_window_t *win);

#endif // _GAMES_GLUTSNAKE_H_