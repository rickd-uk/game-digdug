#include "enemy.h"
#include "grid.h"
#include "player.h"
#include "types.h"
#include <stdlib.h>

void enemy_init(Enemy *enemy, EnemyType type, int col, int row) {
  enemy->col = col;
  enemy->row = row;
  enemy->type = type;
  enemy->facing = DIR_LEFT;
  enemy->is_alive = true;
  enemy->move_slowdown = 0;
  enemy->is_ghosting = false;
}

// Helper; check if tile is walkable for enemies
static bool enemy_can_walk(TileType tile) {
  // enemies can walk through tunnels / empty space
  // and also ghost through dirt
  return (tile == TILE_EMPTY || tile == TILE_TUNNEL || tile == TILE_DIRT);
}

// Helper; get dir to move to target
static Direction get_dir_to(int from_col, int from_row, int to_col,
                            int to_row) {
  // simple pathfinding: move horizontally first, then vertically
  int dx = to_col - from_col;
  int dy = to_row - from_row;

  // prioritize horiz move
  if (abs(dx) > abs(dy)) {
    return (dx > 0) ? DIR_RIGHT : DIR_LEFT;
  } else {
    return (dy > 0) ? DIR_DOWN : DIR_UP;
  }
}

// Helper: Try to move enemy in dir
static Direction enemy_try_move(Enemy *enemy, Direction dir,
                                TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
  int new_col = enemy->col;
  int new_row = enemy->row;

  switch (dir) {
  case DIR_UP:
    new_row--;
    break;
  case DIR_DOWN:
    new_row++;
    break;
  case DIR_LEFT:
    new_col--;
    break;
  case DIR_RIGHT:
    new_col++;
    break;
  }

  // bounds check
  if (new_col < 0 || new_col >= GRID_WIDTH || new_row < 0 ||
      new_row >= GRID_HEIGHT)
    return false;

  TileType tile = grid_get_tile(grid, new_row, new_col);

  // can't move through rocks
  if (tile == TILE_ROCK)
    return false;

  // check if walkable
  if (!enemy_can_walk(tile))
    return false;

  // move is valid
  enemy->col = new_col;
  enemy->row = new_row;
  enemy->facing = dir;

  // are we ghosting through dirt?
  enemy->is_ghosting = (tile == TILE_DIRT);

  // set slowdown based on terrain
  if (tile == TILE_DIRT) {
    enemy->move_slowdown = 20; // slow in dirt
  } else {
    enemy->move_slowdown = 10; // medium speed in tunnels
  }

  return true;
}

void enemy_update(Enemy *enemy, Player *player,
                  TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
  // dead enemies don't move
  if (!enemy->is_alive)
    return;

  // update slowdown
  if (enemy->move_slowdown > 0) {
    enemy->move_slowdown--;
    return; // cannot move yet
  }

  // AI: chase player
  Direction preferred =
      get_dir_to(enemy->col, enemy->row, player->col, player->row);

  // 30% chance that enemy will move randomly
  if (rand() % 100 < 30) {
    Direction rand_dir = rand() % 4;
    if (enemy_try_move(enemy, rand_dir, grid))
      return;
  }

  // try to move in preferred dir
  if (enemy_try_move(enemy, preferred, grid))
    return; // success

  // Blocked! Try other ways
  Direction alternatives[3];
  int alt_count = 0;

  // generate alt. dir (perpendicular + opposite)
  for (Direction d = DIR_UP; d <= DIR_RIGHT; d++) {
    if (d != preferred) {
      alternatives[alt_count++] = d;
    }
  }

  // try random alternetive
  if (alt_count > 0) {
    int choice = rand() % alt_count;
    enemy_try_move(enemy, alternatives[choice], grid);
  }
}

void enemy_get_pixel_pos(Enemy *enemy, int *x, int *y) {
  *x = enemy->col * TILE_SIZE;
  *y = enemy->row * TILE_SIZE;
}

bool enemy_collides_with_player(Enemy *enemy, Player *player) {
  return (enemy->is_alive && player->is_alive && enemy->col == player->col &&
          enemy->row == player->row);
}
