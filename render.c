#include "enemy.h"
#include "player.h"
#include "render.h"
#include "types.h"
#include <SDL3/SDL_render.h>

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

void render_draw_player(SDL_Renderer *renderer, Player *player) {
  SDL_FRect player_rect;

  // get pixel pos
  int x, y;
  player_get_pixel_pos(player, &x, &y);

  player_rect.x = x;
  player_rect.y = y;
  player_rect.w = TILE_SIZE;
  player_rect.h = TILE_SIZE;

  // draw player as white square
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &player_rect);

  // draw a small colored square inside to show dir
  SDL_FRect dir_indicator;
  dir_indicator.w = TILE_INDICATOR_SIZE;
  dir_indicator.h = TILE_INDICATOR_SIZE;

  // pos indicator based on facing dir
  switch (player->facing) {
  case DIR_UP:
    dir_indicator.x = x + TILE_INDICATOR_SIZE;
    dir_indicator.y = y + TILE_INDICATOR_OFFSET;
    break;

  case DIR_DOWN:
    dir_indicator.x = x + TILE_INDICATOR_SIZE;
    dir_indicator.y =
        y + TILE_SIZE - TILE_INDICATOR_OFFSET - TILE_INDICATOR_SIZE;
    break;

  case DIR_LEFT:
    dir_indicator.x = x + TILE_INDICATOR_OFFSET;
    dir_indicator.y = y + TILE_INDICATOR_SIZE;
    break;

  case DIR_RIGHT:
    dir_indicator.x =
        x + TILE_SIZE - TILE_INDICATOR_OFFSET - TILE_INDICATOR_SIZE;
    dir_indicator.y = y + TILE_INDICATOR_SIZE;
    break;
  }

  // draw dir indicator in cyan
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  SDL_RenderFillRect(renderer, &dir_indicator);
}

void render_draw_hud(SDL_Renderer *renderer, Player *player) {
  // draw a simple dir counter as colored bars
  // Each 10 pieces of dirt = 1 bar
  int bars = player->dirt_dug / 10;

  SDL_FRect bar;
  bar.w = 20;
  bar.h = 10;
  bar.y = 5;

  for (int i = 0; i < bars && i < 30; i++) {
    bar.x = 5 + (i * 22);
    SDL_SetRenderDrawColor(renderer, 139, 69, 139, 255); // dirt brown
    SDL_RenderFillRect(renderer, &bar);
  }
}

void render_draw_enemies(SDL_Renderer *renderer, Enemy enemies[],
                         int enemy_count) {
  for (int i = 0; i < enemy_count; i++) {
    Enemy *enemy = &enemies[i];

    if (!enemy->is_alive)
      continue; // skip dead enemies

    SDL_FRect enemy_rect;
    int x, y;
    enemy_get_pixel_pos(enemy, &x, &y);

    enemy_rect.x = x;
    enemy_rect.y = y;
    enemy_rect.w = TILE_SIZE;
    enemy_rect.h = TILE_SIZE;

    // diff colors for diff types
    if (enemy->type == ENEMY_POOKA) {
      // Enemy Pookas: red
      if (enemy->is_ghosting) {
        SDL_SetRenderDrawColor(renderer, 100, 0, 0,
                               180); // semi-transparent red
      } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // bright red
      }
    } else {
      // Enemy Fygar: green

      if (enemy->is_ghosting) {
        SDL_SetRenderDrawColor(renderer, 0, 100, 0,
                               180); // semi-transparent green
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // bright green
      }
    }
    SDL_RenderFillRect(renderer, &enemy_rect);

    // Draw facing indicator (small circle)
    SDL_FRect indicator;
    indicator.w = (float)TILE_SIZE / 4;
    indicator.h = (float)TILE_SIZE / 4;

    // pos based on dir
    switch (enemy->facing) {
    case DIR_UP:
      indicator.x = x + TILE_SIZE / 2 - TILE_SIZE / 8;
      indicator.y = y + 4;
      break;
    case DIR_DOWN:
      indicator.x = x + TILE_SIZE / 2 - TILE_SIZE / 8;
      indicator.y = y + TILE_SIZE - 4 - TILE_SIZE / 4;
      break;

    case DIR_LEFT:
      indicator.x = x + 4;
      indicator.y = y + TILE_SIZE / 2 - TILE_SIZE / 8;
      break;
    case DIR_RIGHT:
      indicator.x = x + TILE_SIZE - 4 - TILE_SIZE / 4;
      indicator.y = y + TILE_SIZE / 2 - TILE_SIZE / 8;
      break;
    }

    // yellow indicator
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &indicator);
  }
}
