#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "grid.h"
#include "player.h"
#include "render.h"
#include "types.h"

int main(int argc, char *argv[]) {
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

  TileType grid[GRID_HEIGHT][GRID_WIDTH];
  grid_init(grid);

  Player player;
  player_init(&player, 10, 2);

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

    // ========= RENDER ========================

    // clear screen with a color (R,G,B,A)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Display the entire grid
    render_draw_grid(renderer, grid);

    // Draw player on top
    render_draw_player(renderer, &player);

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
