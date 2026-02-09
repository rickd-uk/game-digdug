# Step 1: SDL3 Window and Game Loop

## Goal
Create a window, understand the game loop, and handle basic events.

## What You'll Build
A blue window that responds to the ESC key and close button.

## Key Concepts

### 1. SDL3 Initialization
```c
if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
}
```

**What this does:**
- Initializes SDL's video subsystem
- Returns false on failure
- `SDL_GetError()` tells you what went wrong

**Must call this FIRST** before using any SDL functions.

### 2. Creating a Window
```c
SDL_Window* window = SDL_CreateWindow(
    "Dig Dug",          // Title
    640,                // Width
    480,                // Height
    0                   // Flags (0 = default)
);
```

**SDL_Window** is your game window - the canvas you'll draw on.

### 3. Creating a Renderer
```c
SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
```

**SDL_Renderer** is what actually draws to the window. Think of it as your paintbrush.

### 4. The Game Loop

This is the heart of EVERY game:
```c
while (running) {
    // 1. Handle Input
    while (SDL_PollEvent(&event)) {
        // Process keyboard, mouse, window events
    }
    
    // 2. Update Game State
    // (Movement, physics, AI - we'll add this later)
    
    // 3. Render
    SDL_SetRenderDrawColor(renderer, 0, 50, 100, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    // 4. Control Frame Rate
    SDL_Delay(16);  // ~60 FPS
}
```

**Why this pattern?**
- Input: React to what the user does
- Update: Change game state based on input
- Render: Show the current state
- Delay: Don't max out CPU

This loop runs **60 times per second**!

### 5. Event Handling
```c
SDL_Event event;
while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
        running = false;  // User clicked X
    }
    else if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            running = false;  // User pressed ESC
        }
    }
}
```

**SDL_PollEvent()** checks for events in a queue:
- Window close button
- Keyboard presses
- Mouse clicks
- Window resize
- And more...

**Why a while loop?** There might be multiple events in the queue!

### 6. Rendering
```c
// Choose a color (R, G, B, Alpha)
SDL_SetRenderDrawColor(renderer, 0, 50, 100, 255);

// Fill screen with that color
SDL_RenderClear(renderer);

// Show what we drew (flip the buffer)
SDL_RenderPresent(renderer);
```

**SDL_RenderPresent()** is crucial - without it, nothing shows! It's like flipping pages in a flipbook.

### 7. Frame Rate Control
```c
SDL_Delay(16);  // Wait 16 milliseconds
```

**Why?**
- 1000ms / 60 FPS = ~16.67ms per frame
- Without delay, loop runs millions of times per second
- Wastes CPU and makes timing unpredictable

Common frame rates:
- 60 FPS = `SDL_Delay(16)`
- 30 FPS = `SDL_Delay(33)`
- 120 FPS = `SDL_Delay(8)`

### 8. Cleanup
```c
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
```

**CRITICAL:** Always clean up in **REVERSE order** of creation:
1. Renderer (last created)
2. Window (second created)
3. SDL_Quit() (first initialized)

## Complete Example
```c
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    // Initialize
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Dig Dug", 640, 480, 0);
    if (!window) {
        fprintf(stderr, "Window failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        fprintf(stderr, "Renderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Game loop
    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 50, 100, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        
        // Frame rate
        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
```

## Experiments

1. **Change window size**: Try 800x600 or 1024x768
2. **Change background color**: Try (255, 0, 0, 255) for red
3. **Different key**: Use `SDLK_SPACE` instead of `SDLK_ESCAPE`
4. **Slower FPS**: Change `SDL_Delay(16)` to `SDL_Delay(100)`

## Common Mistakes

❌ **Forgetting to check errors**
```c
SDL_Window* window = SDL_CreateWindow(...);
// Always check if window == NULL!
```

❌ **Wrong cleanup order**
```c
SDL_Quit();
SDL_DestroyWindow(window);  // WRONG - SDL already quit!
```

❌ **Forgetting SDL_RenderPresent()**
```c
SDL_RenderClear(renderer);
// Nothing shows without SDL_RenderPresent()!
```

❌ **Not calling SDL_PollEvent in a loop**
```c
SDL_PollEvent(&event);  // WRONG - misses events!
// Should be: while (SDL_PollEvent(&event))
```

## Key Takeaways

✅ SDL3 must be initialized before use  
✅ Game loop = Input → Update → Render → Delay  
✅ Events go in a queue - process them in a loop  
✅ SDL_RenderPresent() makes drawings visible  
✅ Frame rate control prevents CPU waste  
✅ Always clean up in reverse order  

## Next Step

**Step 2**: Learn to draw rectangles - the building blocks of tiles!
