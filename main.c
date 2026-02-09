#include "enemy.h"
#include "grid.h"
#include "player.h"
#include "render.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  // initialize SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }
  // create window
  SDL_Window *window =
      SDL_CreateWindow("Dig dug - Step 1: Window", SCREEN_WIDTH, SCREEN_HEIGHT,
                       0 // Flags
      );
  if (!window) {
    fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // create a renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  if (!renderer) {
    fprintf(stderr, "Render creation failed: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  printf("SDL3 Initialized successfully!\n");
  printf("Press ESC or close window to quit\n");

  // seed random num gen
  srand(time(NULL));

  TileType grid[GRID_HEIGHT][GRID_WIDTH];
  grid_init(grid);

  Player player;
  player_init(&player, 10, 2);

  // create enemies
  Enemy enemies[MAX_ENEMIES];
  int enemy_count = 0;

  // spawn 2 Pookas  & 1 Fygar
  enemy_init(&enemies[enemy_count++], ENEMY_POOKA, 20, 5);
  // enemy_init(&enemies[enemy_count++], ENEMY_POOKA, 5, 10);
  // enemy_init(&enemies[enemy_count++], ENEMY_FYGAR, 10, 8);

  // Game loop control
  bool running = true;
  SDL_Event event;

  // main game loop
  while (running) {
    // handle events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        // use clicked x
        running = false;
      } else if (event.type == SDL_EVENT_KEY_DOWN) {
        // user pressed a key
        if (event.key.key == SDLK_ESCAPE) {
          running = false;
        } else if (event.key.key == SDLK_UP) {
          player_move(&player, DIR_UP, grid);
        } else if (event.key.key == SDLK_DOWN) {
          player_move(&player, DIR_DOWN, grid);
        } else if (event.key.key == SDLK_LEFT) {
          player_move(&player, DIR_LEFT, grid);
        } else if (event.key.key == SDLK_RIGHT) {
          player_move(&player, DIR_RIGHT, grid);
        }
      }
    }

    // ========= UPDATE (game Logic) ===========
    player_update(&player);

    // Update all enenmies
    for (int i = 0; i < enemy_count; i++) {
      enemy_update(&enemies[i], &player, grid);

      // check collision with player
      if (enemy_collides_with_player(&enemies[i], &player)) {
        printf("Hit by enemy!! Game over!\n");
        player.is_alive = false;
      }
    }
    // ========= RENDER ========================

    // clear screen with a color (R,G,B,A)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Display the entire grid
    render_draw_grid(renderer, grid);

    // Display enemies
    render_draw_enemies(renderer, enemies, enemy_count);

    // Draw player on top
    render_draw_player(renderer, &player);

    // Draw HUD - dirt bar
    render_draw_hud(renderer, &player);

    // Present
    SDL_RenderPresent(renderer);

    // Small delay to not max out CPU
    SDL_Delay(16); // 60 FPD (1000ms/60 ≈ 16ms);
  }

  // cleanup - Always in reverse order
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  printf("Goodbye!\n");
  return 0;
}
