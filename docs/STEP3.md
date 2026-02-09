# Step 3: Full Tile Grid System & Modular Code

## Goal
Create a complete tile-based world and organize code into reusable modules.

## What You'll Build
A 20×15 tile grid representing the entire Dig Dug underground, split across multiple well-organized files.

## Part A: The Tile Grid

### 1. 2D Arrays - Your Game World
```c
#define GRID_WIDTH 20
#define GRID_HEIGHT 15

TileType grid[GRID_HEIGHT][GRID_WIDTH];
```

**Understanding the syntax:**
- `grid[row][column]` = `grid[y][x]`
- **Always row first, then column** (like a spreadsheet)
- Total tiles: 20 × 15 = 300 tiles

**Memory layout:**
```
grid[0][0], grid[0][1], grid[0][2], ... grid[0][19]  ← Row 0
grid[1][0], grid[1][1], grid[1][2], ... grid[1][19]  ← Row 1
...
grid[14][0], grid[14][1], ... grid[14][19]           ← Row 14
```

**Accessing specific tiles:**
```c
grid[0][0] = TILE_DIRT;        // Top-left
grid[5][10] = TILE_ROCK;       // Middle
grid[14][19] = TILE_TUNNEL;    // Bottom-right
```

### 2. Enums - Tile Types
```c
typedef enum {
    TILE_EMPTY = 0,   // Black - nothing there
    TILE_DIRT,        // Brown - solid dirt
    TILE_TUNNEL,      // Dark brown - dug out path
    TILE_ROCK         // Gray - falling rock
} TileType;
```

**Why enums instead of numbers?**

❌ Bad (magic numbers):
```c
grid[5][10] = 3;  // What does 3 mean?
```

✅ Good (enum):
```c
grid[5][10] = TILE_ROCK;  // Crystal clear!
```

### 3. Initializing the Grid
```c
void grid_init(TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    for (int row = 0; row < GRID_HEIGHT; row++) {
        for (int col = 0; col < GRID_WIDTH; col++) {
            if (row < 2) {
                grid[row][col] = TILE_EMPTY;  // Surface
            }
            else if (row == 2 && col >= 5 && col <= 14) {
                grid[row][col] = TILE_TUNNEL;  // Pre-dug tunnel
            }
            else {
                grid[row][col] = TILE_DIRT;  // Underground
            }
        }
    }
}
```

**Nested loop pattern:**
- Outer loop: Goes through each row (top to bottom)
- Inner loop: Goes through each column (left to right)
- This visits EVERY tile once

### 4. Drawing the Grid
```c
void render_draw_grid(SDL_Renderer* renderer, 
                      TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    SDL_FRect tile;
    tile.w = TILE_SIZE;
    tile.h = TILE_SIZE;
    
    for (int row = 0; row < GRID_HEIGHT; row++) {
        for (int col = 0; col < GRID_WIDTH; col++) {
            // Convert grid coords to pixel coords
            tile.x = col * TILE_SIZE;
            tile.y = row * TILE_SIZE;
            
            // Get tile type
            TileType type = grid[row][col];
            
            // Get color for this type
            int r, g, b;
            get_tile_color(type, &r, &g, &b);
            
            // Draw it
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}
```

### 5. Grid ↔ Pixel Conversion
```c
// Grid position to pixel position
pixel_x = col * TILE_SIZE;
pixel_y = row * TILE_SIZE;

// Pixel position to grid position
col = pixel_x / TILE_SIZE;
row = pixel_y / TILE_SIZE;
```

**Example with TILE_SIZE = 32:**
- Grid (5, 3) → Pixel (160, 96)
- Pixel (200, 150) → Grid (6, 4)

## Part B: Modular Code Organization

### Why Split Into Multiple Files?

**Before (1 big file):**
- 500+ lines in main.c
- Hard to find anything
- Changes affect everything
- Can't reuse code

**After (modules):**
- Each file has ONE job
- Easy to find code
- Changes are isolated
- Can reuse modules

### File Structure
```
digdug/
├── main.c           # Game loop coordination
├── types.h          # Shared types and constants
├── grid.h           # Grid function declarations
├── grid.c           # Grid function implementations
├── render.h         # Render function declarations
├── render.c         # Render function implementations
└── Makefile         # Build instructions
```

### types.h - Shared Definitions
```c
#ifndef TYPES_H
#define TYPES_H

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRID_WIDTH 20
#define GRID_HEIGHT 15
#define TILE_SIZE 32

typedef enum {
    TILE_EMPTY = 0,
    TILE_DIRT,
    TILE_TUNNEL,
    TILE_ROCK
} TileType;

#endif // TYPES_H
```

**Header guards explained:**
```c
#ifndef TYPES_H     // If not defined
#define TYPES_H     // Define it now
// ... code ...
#endif              // End if
```

**Why?** Prevents double inclusion:
```c
#include "types.h"
#include "grid.h"   // grid.h also includes types.h
// Without guards: TileType defined twice = ERROR
// With guards: Second include skipped = OK
```

### grid.h - Grid Interface
```c
#ifndef GRID_H
#define GRID_H

#include "types.h"

// Initialize grid with starting level
void grid_init(TileType grid[GRID_HEIGHT][GRID_WIDTH]);

// Get tile at position (with bounds checking)
TileType grid_get_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], 
                       int row, int col);

// Set tile at position (with bounds checking)
void grid_set_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], 
                   int row, int col, TileType type);

#endif // GRID_H
```

**.h file = "What can I do?"**
- Function declarations only
- No implementation details
- Other files include this to use grid functions

### grid.c - Grid Implementation
```c
#include "grid.h"

void grid_init(TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    // Implementation here
}

TileType grid_get_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], 
                       int row, int col) {
    // Bounds checking
    if (row < 0 || row >= GRID_HEIGHT || 
        col < 0 || col >= GRID_WIDTH) {
        return TILE_EMPTY;  // Safe default
    }
    return grid[row][col];
}

void grid_set_tile(TileType grid[GRID_HEIGHT][GRID_WIDTH], 
                   int row, int col, TileType type) {
    // Bounds checking
    if (row < 0 || row >= GRID_HEIGHT || 
        col < 0 || col >= GRID_WIDTH) {
        return;  // Ignore out-of-bounds
    }
    grid[row][col] = type;
}
```

**.c file = "How do I do it?"**
- Actual function implementations
- Hidden implementation details
- Only compiled once

### render.h/render.c - Rendering Module

**render.h:**
```c
#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include "types.h"

void render_get_tile_color(TileType type, int* r, int* g, int* b);
void render_draw_grid(SDL_Renderer* renderer, 
                      TileType grid[GRID_HEIGHT][GRID_WIDTH]);

#endif // RENDER_H
```

**render.c:**
```c
#include "render.h"

void render_get_tile_color(TileType type, int* r, int* g, int* b) {
    switch(type) {
        case TILE_EMPTY:
            *r = 0; *g = 0; *b = 0;
            break;
        case TILE_DIRT:
            *r = 139; *g = 69; *b = 19;
            break;
        case TILE_TUNNEL:
            *r = 50; *g = 25; *b = 10;
            break;
        case TILE_ROCK:
            *r = 128; *g = 128; *b = 128;
            break;
    }
}

void render_draw_grid(SDL_Renderer* renderer, 
                      TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    // Implementation from earlier
}
```

### main.c - The Coordinator
```c
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "types.h"
#include "grid.h"
#include "render.h"

int main(void) {
    // [SDL initialization]
    
    // Create grid
    TileType grid[GRID_HEIGHT][GRID_WIDTH];
    grid_init(grid);
    
    // Game loop
    while (running) {
        // [Event handling]
        
        // Render
        SDL_RenderClear(renderer);
        render_draw_grid(renderer, grid);  // Draw entire grid
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16);
    }
    
    // [Cleanup]
    return 0;
}
```

**main.c's job:**
- Initialize SDL
- Create game objects
- Run the game loop
- Coordinate modules
- Clean up

**main.c does NOT:**
- Know how grid works internally
- Know how rendering works
- Contain game logic

### Compilation Process
```
Step 1: Compile each .c file to .o file
  main.c    → main.o
  grid.c    → grid.o
  render.c  → render.o

Step 2: Link all .o files together
  main.o + grid.o + render.o → digdug executable
```

**Makefile automates this:**
```makefile
SOURCES = main.c grid.c render.c
OBJECTS = main.o grid.o render.o

$(TARGET): $(OBJECTS)
    gcc $(OBJECTS) -lSDL3 -o $(TARGET)

main.o: main.c types.h grid.h render.h
    gcc -c main.c -o main.o

grid.o: grid.c types.h grid.h
    gcc -c grid.c -o grid.o

render.o: render.c types.h render.h
    gcc -c render.c -o render.o
```

**Dependency tracking:**
- If types.h changes → recompile everything
- If grid.c changes → recompile only grid.o
- If main.c changes → recompile only main.o

## Key Concepts

### Separation of Concerns

Each module has ONE job:
- **grid.c**: "What is the world?"
- **render.c**: "How do we show it?"
- **main.c**: "How does it work together?"

### Single Responsibility Principle
```c
// ❌ BAD: render.c modifying game state
void render_draw_player(...) {
    player.score += 10;  // NO! Rendering shouldn't change state
}

// ✅ GOOD: render.c only draws
void render_draw_player(...) {
    // Just draw, don't modify anything
}
```

### One-Way Dependencies
```
main.c
  ↓
  ├→ grid.c
  ├→ render.c
      ↓
      └→ grid.c (render needs to know about grid)

grid.c does NOT depend on render.c!
```

**Why?** Grid logic works the same regardless of how it's rendered (SDL, OpenGL, terminal, etc.)

## Experiments

1. **Add a new tile type**
   - Add `TILE_WATER` to enum in types.h
   - Add color case in render.c
   - Place water tiles in grid_init()

2. **Create different level patterns**
   - Vertical tunnels
   - Maze-like structures
   - Rooms connected by passages

3. **Add grid helper functions**
```c
   // In grid.h
   int grid_count_dirt(TileType grid[GRID_HEIGHT][GRID_WIDTH]);
   bool grid_is_tunnel(TileType grid[GRID_HEIGHT][GRID_WIDTH], 
                       int row, int col);
```

4. **Visualize grid coordinates**
   - Print grid[row][col] values
   - Draw grid lines between tiles

## Common Mistakes

❌ **Circular includes**
```c
// grid.h includes render.h
// render.h includes grid.h
// ERROR: Infinite loop!
```

❌ **Mixing concerns**
```c
// grid.c should NOT have SDL code!
void grid_draw(...) {  // Wrong file!
    SDL_RenderFillRect(...);
}
```

❌ **Forgetting header guards**
```c
// types.h without guards
typedef enum { ... } TileType;
// Gets defined multiple times = ERROR
```

❌ **Wrong array indexing**
```c
grid[col][row]  // WRONG
grid[row][col]  // CORRECT
```

## Key Takeaways

✅ 2D arrays represent tile grids  
✅ Nested loops process every tile  
✅ Enums make code readable  
✅ Grid coords convert to pixels  
✅ Modular code is easier to maintain  
✅ Header guards prevent double-inclusion  
✅ Each module has one responsibility  
✅ Dependencies should be one-way  

## Next Step

**Step 4**: Add a player character that moves through the grid!
