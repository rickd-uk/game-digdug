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
    SDL_SetRenderDrawColor(renderer, 0, 50, 100, 255);
    SDL_RenderClear(renderer);

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
