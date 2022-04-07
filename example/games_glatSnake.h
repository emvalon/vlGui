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