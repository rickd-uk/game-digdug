#ifndef GRID_H
#define GRID_H

#include "types.h"

// initialize grid with a starting pattern
void grid_init(TileType grid[GRID_HEIGHT][GRID_WIDTH]);

// get tile type at specific pos.
TileType grid_get_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], int row,
                       int col);

// set tile at specific pos.
void grid_set_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], int row, int col,
                   TileType type);

#endif
