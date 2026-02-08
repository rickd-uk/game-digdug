#include "grid.h"

void grid_init(TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      if (row < 2) {
        // top 2 rows are empty (sky/surface)
        grid[row][col] = TILE_EMPTY;
      } else if (row == 2) {
        // row 2 has some tunnels pre-dug
        if (col >= 5 && col <= 14) {
          grid[row][col] = TILE_TUNNEL;
        } else {
          grid[row][col] = TILE_DIRT;
        }
      } else if (row == 5 && col == 10) {
        // add a single rock
        grid[row][col] = TILE_ROCK;
      } else {
        // everything else is dirt
        grid[row][col] = TILE_DIRT;
      }
    }
  }
}

TileType grid_get_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], int row,
                       int col) {
  if (row < 0 || row >= GRID_HEIGHT || col < 0 || col >= GRID_WIDTH) {
    return TILE_EMPTY; // safe default
  }
  return grid[row][col];
}

void grid_set_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], int row, int col,
                   TileType type) {

  if (row < 0 || row >= GRID_HEIGHT || col < 0 || col >= GRID_WIDTH) {
    return;
  }
  grid[row][col] = type;
}
