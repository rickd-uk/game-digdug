# Step 2: Drawing Rectangles

## Goal
Learn to draw rectangles - the building blocks of all tile-based games.

## What You'll Build
A window with multiple colored rectangles representing different tile types.

## Key Concepts

### 1. SDL_FRect Structure
```c
SDL_FRect rect = {
    .x = 100,    // Position from left edge (pixels)
    .y = 100,    // Position from top edge (pixels)
    .w = 32,     // Width in pixels
    .h = 32      // Height in pixels
};
```

**What is SDL_FRect?**
- A rectangle definition with floats
- Stores position (x, y) and size (w, h)
- The 'F' means it uses floats instead of integers

**Coordinate System:**
```
(0,0)  ───────────> X axis
  │
  │
  │
  ▼
Y axis

Top-left is (0, 0)
X increases to the right
Y increases downward
```

### 2. Drawing a Rectangle
```c
// Step 1: Choose a color
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red

// Step 2: Draw filled rectangle
SDL_RenderFillRect(renderer, &rect);
```

**Color Format: RGBA**
- R = Red (0-255)
- G = Green (0-255)
- B = Blue (0-255)
- A = Alpha/transparency (255 = opaque)

**The `&` Symbol:**
```c
SDL_RenderFillRect(renderer, &rect);
//                            ^ This means "address of rect"
```
SDL needs to know WHERE the rect data is in memory (pointer).

### 3. Common Colors
```c
// Primary colors
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);     // Red
SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);     // Green
SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);     // Blue

// Dig Dug colors
SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);   // Brown (dirt)
SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray (rock)
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);       // Black (empty)

// Mixed colors
SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);   // Yellow (R+G)
SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);   // Magenta (R+B)
SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);   // Cyan (G+B)
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White (all)
```

### 4. Drawing Multiple Rectangles

**Method 1: Individual rectangles**
```c
SDL_FRect dirt = {.x = 100, .y = 100, .w = 32, .h = 32};
SDL_FRect rock = {.x = 150, .y = 100, .w = 32, .h = 32};

SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
SDL_RenderFillRect(renderer, &dirt);

SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
SDL_RenderFillRect(renderer, &rock);
```

**Method 2: Array of rectangles (better!)**
```c
SDL_FRect tiles[5];
for (int i = 0; i < 5; i++) {
    tiles[i].x = 100 + (i * 32);  // Space them 32 pixels apart
    tiles[i].y = 200;
    tiles[i].w = 32;
    tiles[i].h = 32;
}

// Draw all tiles
SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
for (int i = 0; i < 5; i++) {
    SDL_RenderFillRect(renderer, &tiles[i]);
}
```

### 5. Drawing Order Matters!
```c
// Background drawn first (back layer)
SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
SDL_RenderFillRect(renderer, &background);

// Foreground drawn last (front layer)
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_RenderFillRect(renderer, &foreground);
```

Think of it like painting:
- First paint = back layer
- Last paint = front layer
- Things drawn last appear on top

### 6. Tile Size Convention
```c
#define TILE_SIZE 32  // Common for retro games
```

**Why 32?**
- Divides evenly into common resolutions
- 640 / 32 = 20 tiles wide
- 480 / 32 = 15 tiles tall
- Easy to work with (power of 2)

Other common sizes: 16, 24, 32, 48, 64

## Complete Example
```c
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define TILE_SIZE 32

int main(void) {
    // [SDL initialization code from Step 1]
    
    // Create tiles for different types
    SDL_FRect dirt_tile = {.x = 100, .y = 100, .w = TILE_SIZE, .h = TILE_SIZE};
    SDL_FRect rock_tile = {.x = 150, .y = 100, .w = TILE_SIZE, .h = TILE_SIZE};
    SDL_FRect tunnel_tile = {.x = 200, .y = 100, .w = TILE_SIZE, .h = TILE_SIZE};
    
    // Create a row of tiles
    SDL_FRect tile_row[5];
    for (int i = 0; i < 5; i++) {
        tile_row[i].x = 100 + (i * TILE_SIZE);
        tile_row[i].y = 200;
        tile_row[i].w = TILE_SIZE;
        tile_row[i].h = TILE_SIZE;
    }
    
    bool running = true;
    SDL_Event event;
    
    while (running) {
        // [Event handling from Step 1]
        
        // Clear screen (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Draw dirt tile (brown)
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        SDL_RenderFillRect(renderer, &dirt_tile);
        
        // Draw rock tile (gray)
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &rock_tile);
        
        // Draw tunnel tile (dark brown)
        SDL_SetRenderDrawColor(renderer, 50, 25, 10, 255);
        SDL_RenderFillRect(renderer, &tunnel_tile);
        
        // Draw row of dirt tiles
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        for (int i = 0; i < 5; i++) {
            SDL_RenderFillRect(renderer, &tile_row[i]);
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    // [Cleanup from Step 1]
    return 0;
}
```

## Experiments

1. **Create a checkerboard pattern**
```c
   for (int row = 0; row < 10; row++) {
       for (int col = 0; col < 10; col++) {
           if ((row + col) % 2 == 0) {
               // Draw white tile
           } else {
               // Draw black tile
           }
       }
   }
```

2. **Animate a moving rectangle**
```c
   SDL_FRect moving_tile = {.x = 0, .y = 100, .w = 32, .h = 32};
   
   // In game loop:
   moving_tile.x += 1;  // Move 1 pixel per frame
   if (moving_tile.x > 640) {
       moving_tile.x = 0;  // Wrap around
   }
```

3. **Draw a border around tiles**
```c
   // After drawing filled rect, draw outline
   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White
   SDL_RenderRect(renderer, &tile);  // Outline only
```

4. **Try different tile sizes**
   - Change `TILE_SIZE` to 16, 24, 48, or 64
   - See how it affects the visual scale

## Understanding the Math

**Spacing tiles horizontally:**
```c
x_position = start_x + (tile_index * TILE_SIZE)
```

Example with TILE_SIZE = 32, start_x = 100:
- Tile 0: x = 100 + (0 × 32) = 100
- Tile 1: x = 100 + (1 × 32) = 132
- Tile 2: x = 100 + (2 × 32) = 164

**Creating a grid:**
```c
for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
        tile.x = col * TILE_SIZE;
        tile.y = row * TILE_SIZE;
        SDL_RenderFillRect(renderer, &tile);
    }
}
```

## Common Mistakes

❌ **Setting color after drawing**
```c
SDL_RenderFillRect(renderer, &rect);
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Too late!
```

❌ **Forgetting the `&` in SDL_RenderFillRect**
```c
SDL_RenderFillRect(renderer, rect);  // WRONG - need &rect
```

❌ **Drawing outside screen bounds**
```c
// If screen is 640x480 and tile is 32x32:
rect.x = 650;  // Will be off-screen!
```

## Key Takeaways

✅ SDL_FRect defines rectangles with position and size  
✅ Color must be set BEFORE drawing  
✅ Arrays + loops = efficient tile drawing  
✅ Drawing order determines layering  
✅ Use consistent tile sizes (powers of 2 are best)  
✅ Position math: `start + (index × size)`  

## Next Step

**Step 3**: Create a full tile grid - the complete game world!
