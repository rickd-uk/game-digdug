#ifndef TYPES_H
#define TYPES_H

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define GRID_WIDTH 20
#define GRID_HEIGHT 15
#define TILE_SIZE 32
#define TILE_INDICATOR_SIZE 10
#define TILE_INDICATOR_OFFSET 2 // margin from edge

typedef enum {
  TILE_EMPTY = 0, // Black
  TILE_DIRT,
  TILE_TUNNEL,
  TILE_ROCK,
} TileType;

#endif
