#ifndef ENEMY_H
#define ENEMY_H

#include "player.h"
#include "types.h"
#include <stdbool.h>

#define MAX_ENEMIES 10

typedef enum { ENEMY_POOKA, ENEMY_FYGAR } EnemyType;

typedef struct {
  int col;
  int row;
  EnemyType type;
  Direction facing;
  bool is_alive;
  int move_slowdown;
  bool is_ghosting;
} Enemy;

void enemy_init(Enemy *enemy, EnemyType type, int col, int row);

// update enemy AI
void enemy_update(Enemy *enemy, Player *player,
                  TileType grid[GRID_HEIGHT][GRID_WIDTH]);

// get pixel pos for rendering
void enemy_get_pixel_pos(Enemy *enemy, int *x, int *y);

bool enemy_collides_with_player(Enemy *enemy, Player *player);

#endif
