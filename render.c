#include "render.h"

void render_get_tile_color(TileType type, int *r, int *g, int *b) {
  switch (type) {
  case TILE_EMPTY:
    *r = 0;
    *g = 0;
    *b = 0; // black
    break;
  case TILE_DIRT:
    *r = 139;
    *g = 69;
    *b = 19; // light brown
    break;
  case TILE_TUNNEL:
    *r = 50;
    *g = 25;
    *b = 10; // dark brown
    break;
  case TILE_ROCK:
    *r = 128;
    *g = 128;
    *b = 128; // gray
    break;

  default:
    *r = 255;
    *g = 0;
    *b = 255; // gray
  }
}

void render_draw_grid(SDL_Renderer *renderer,
                      TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
  SDL_FRect tile;
  tile.w = TILE_SIZE;
  tile.h = TILE_SIZE;

  for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
      // convert grid pos to pixel pos
      tile.x = col * TILE_SIZE;
      tile.y = row * TILE_SIZE;

      // get tile at this pos
      TileType type = grid[row][col];

      int r, g, b;
      render_get_tile_color(type, &r, &g, &b);

      // draw them
      SDL_SetRenderDrawColor(renderer, r, g, b, 255);
      SDL_RenderFillRect(renderer, &tile);
    }
  }
}
