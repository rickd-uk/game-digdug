#include "grid.h"
#include "player.h"
#include "types.h"

void player_init(Player *player, int start_col, int start_row) {
  player->col = start_col;
  player->row = start_row;
  player->facing = DIR_RIGHT;
  player->is_alive = true;
  player->dirt_dug = 0;
  player->move_slowdown = 0;
}

static bool can_move_to(TileType tile) {
  // player can move thru empty space & tunnels
  // but not dirt / rocks
  return (tile == TILE_EMPTY || tile == TILE_TUNNEL);
}

bool player_move(Player *player, Direction dir,
                 TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
  // check slowdown - cannot move yet
  if (player->move_slowdown > 0)
    return false;

  int new_col = player->col;
  int new_row = player->row;

  // calc new pos based on dir
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

  if (new_col < 0 || new_col >= GRID_WIDTH || new_row < 0 ||
      new_row >= GRID_HEIGHT) {
    return false;
  }

  // check if destantion file is walkable
  TileType destination = grid_get_tile(grid, new_row, new_col);
  // cannot dig upwards
  if (destination == TILE_DIRT && dir == DIR_UP)
    return false;

  bool just_dug = false;
  // remove dirt if there is some
  if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    destination = TILE_TUNNEL; // update local view
    just_dug = true;
    player->dirt_dug++;
  }

  if (!can_move_to(destination))
    return false; // blocked by rock

  // move is valid - update pos
  player->col = new_col;
  player->row = new_row;
  player->facing = dir;

  // set movement slowdown based on what happened
  if (just_dug) {
    player->move_slowdown = 8; // slower when digging (8 frames)
  } else {
    player->move_slowdown = 3; // faster in tunnels
  }

  return true;
}

void player_get_pixel_pos(Player *player, int *x, int *y) {
  *x = player->col * TILE_SIZE;
  *y = player->row * TILE_SIZE;
}

void player_update(Player *player) {
  if (player->move_slowdown > 0) {
    player->move_slowdown--;
  }
}
