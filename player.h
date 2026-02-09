#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"
#include <stdbool.h>

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

typedef struct {
  int col;
  int row;
  Direction facing;
  bool is_alive;
} Player;

void player_init(Player *player, int start_col, int start_row);

bool player_move(Player *player, Direction dir,
                 TileType grid[GRID_HEIGHT][GRID_WIDTH]);

void player_get_pixel_pos(Player *player, int *x, int *y);

#endif
