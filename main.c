#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char *argv[]) {
  // initialize SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }
  // create window
  SDL_Window *window =
      SDL_CreateWindow("Dig dug - Step 1: Window", WIDTH, HEIGHT,
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

  SDL_FRect dirt_block = {.x = 100, .y = 100, .w = 32, .h = 32};

  SDL_FRect rock = {.x = 150, .y = 100, .w = 32, .h = 32};
  SDL_FRect tunnel = {.x = 200, .y = 100, .w = 32, .h = 32};

  // create a row of dirt blocks
  SDL_FRect dirt_row[5];
  for (int i = 0; i < 5; i++) {
    dirt_row[i].x = 100 + (i * 32); // space them 32 pixels apart
    dirt_row[i].y = 200;
    dirt_row[i].w = 32;
    dirt_row[i].h = 32;
  }
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
        }
      }
    }
    // clear screen with a color (R,G,B,A)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_RenderFillRect(renderer, &dirt_block);

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &rock);

    SDL_SetRenderDrawColor(renderer, 50, 25, 10, 255);
    SDL_RenderFillRect(renderer, &tunnel);

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    for (int i = 0; i < 5; i++) {
      SDL_RenderFillRect(renderer, &dirt_row[i]);
    }
    // Display what we draw
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
