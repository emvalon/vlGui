/**
 * @file games_glatSnake.c
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
#include <stdio.h>
#include "games_glatSnake.h"
#include "vlGui_input.h"
#include "vlGui_base.h"

#define GAMES_GLUTSNAKE_BASE_X      1
#define GAMES_GLUTSNAKE_BASE_Y      1


static struct game_glutSnake_t g_game_glutSnake;
static bool g_game_drawBkg = true;

static void 
games_init(struct game_glutSnake_t * game){
	game->apple_pos[0] = 10;
	game->apple_pos[1] = 10;
    
	game->player.lenght = 1;
	game->player.x_dir = 1;
	game->player.y_dir = 0;
	game->player.tailPos[0][0] = 0;
	game->player.tailPos[0][1] = 0;
};

static void 
games_updateGame(struct game_glutSnake_t* game) {
    // Check if the player is on the apple
	if((game->player.tailPos[0][0] == game->apple_pos[0])){
		if(game->player.tailPos[0][1] == game->apple_pos[1]){
			game->player.lenght++;
			game->apple_pos[0] = rand() % 20;
			game->apple_pos[1] = rand() % 20;
		}
	}

	// Move the snake tail unless the head
	for(int i = (game->player.lenght - 1); i > 0; i--) {
		game->player.tailPos[i][0] = 
			game->player.tailPos[i - 1][0];
		game->player.tailPos[i][1] = 
			game->player.tailPos[i - 1][1];
	};
	// Update player position
	game->player.tailPos[0][0] += game->player.x_dir;
	game->player.tailPos[0][1] += game->player.y_dir;
	// If the player is on the border of the screen
	if(game->player.tailPos[0][0] < 0) {
		game->player.tailPos[0][0] = GAMES_SCREEN_WIDTH - 1; 
	}
	else if(game->player.tailPos[0][0] >= GAMES_SCREEN_WIDTH){
		game->player.tailPos[0][0] = 0;
	}
	if(game->player.tailPos[0][1] < 0) {
		game->player.tailPos[0][1] = GAMES_SCREEN_HEIGTH - 1; 
	}
	else if(game->player.tailPos[0][1] >= GAMES_SCREEN_HEIGTH) {
		game->player.tailPos[0][1] = 0;
	}
	
	for(int i = 1; i < game->player.lenght; i++) {
		if(game->player.tailPos[i][0] == game->player.tailPos[0][0]){
			if(game->player.tailPos[i][1] == game->player.tailPos[0][1]){
				games_init(&(*game));
			}
		}
	};
}


static int 
games_glutSnakeKeyCb(struct vlGui_window_t *win, uint8_t key)
{
    struct game_Snake_t *player;

    player = &g_game_glutSnake.player;

	if((key == VLGUI_KEY_UP) && (player->y_dir == 0)) {
		player->x_dir = 0;
		player->y_dir = -1;
	}
	else if((key == VLGUI_KEY_DOWN) && (player->y_dir == 0)) {
		player->x_dir = 0;
		player->y_dir = 1;
	}
	else if((key == VLGUI_KEY_LETF) && (player->x_dir == 0)) {
		player->x_dir = -1;
		player->y_dir = 0;
	}
	else if((key == VLGUI_KEY_RIGHT) && (player->x_dir == 0)) {
		player->x_dir = 1;
		player->y_dir = 0;
	}
}


static void 
games_bkgDrawCb(struct vlGui_window_t *win, void *arg)
{
    char score[4];
    char level[2];

    if (g_game_drawBkg) {
        vlGui_windowClear(win);
        vlGui_drawRectangle(win, 0, 1, GAMES_SCREEN_WIDTH * 3 + 2, GAMES_SCREEN_HEIGTH * 3 + 2, 1);
        vlGui_setFont(&vlGui_font6x8);
        vlGui_drawString(win, 95, 10, "Score", 1);
        vlGui_drawString(win, 95, 40, "level", 1);
        g_game_drawBkg = false;
    } else {
        vlGui_drawBlock(win, 107, 20, 18, 8, 0);
        vlGui_drawBlock(win, 119, 50, 6, 8, 0);
    }

    snprintf(score, sizeof(score), "%03d", g_game_glutSnake.player.lenght - 1);
    vlGui_drawString(win, 107, 20, score, 1);
    snprintf(level, sizeof(level), "%01d", g_game_glutSnake.level);
    vlGui_drawString(win, 119, 50, level, 1);
}

static void 
games_glutSnakeDrawCb(struct vlGui_window_t *win, void *arg)
{
    static uint8_t cnt = 0;
    struct game_glutSnake_t *game;

    if (++cnt >= 5) {
        cnt = 0;

        game = &g_game_glutSnake;

        vlGui_windowClear(win);

        for(int i = 0; i < (game->player.lenght); i++){
            vlGui_drawBlock(win, game->player.tailPos[i][0] * 3, game->player.tailPos[i][1] * 3, 3, 3, 1);
        }

        vlGui_drawRectangle(win, game->apple_pos[0] * 3, game->apple_pos[1] * 3, 3, 3, 1);

        games_updateGame(game);
    }
}

int
games_gultSnakeCreate(struct vlGui_window_t *win)
{
    struct vlGui_window_t *main_win;
    struct vlGui_window_t *game_win;


    main_win = vlGui_windowCreate(win, 0, 0, win->win_width, win->win_height, 0);
    if (main_win == NULL) {
        return -1;
    }

    game_win = vlGui_windowCreate(win, 1, 2,  GAMES_SCREEN_WIDTH * 3, GAMES_SCREEN_HEIGTH * 3, 0);
    if (game_win == NULL) {
        vlGui_windowDeleteChildren(win);
        return -1;
    }

    g_game_glutSnake.level = 1;
	games_init(&g_game_glutSnake);
    g_game_drawBkg = true;

    vlGui_windowSetDrawCb(main_win, games_bkgDrawCb);
    vlGui_windowSetKeyCb(main_win, games_glutSnakeKeyCb);

    vlGui_windowSetDrawCb(game_win, games_glutSnakeDrawCb);

    return 0;
}
