#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include <SDL3/SDL.h>

// forward declare player struct (defined in player.h)
typedef struct Player player;

// get RGB color for a tile type
void render_get_tile_color(TileType type, int *r, int *g, int *b);

// draw whole grid
void render_draw_grid(SDL_Renderer *renderer,
                      TileType grid[GRID_HEIGHT][GRID_WIDTH]);

// draw player
void render_draw_player(SDL_Renderer *renderer, Player *player);

#endif
