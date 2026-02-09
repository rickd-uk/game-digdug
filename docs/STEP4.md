# Step 4: Player Character & Movement

## Goal
Add a controllable character with keyboard movement and collision detection.

## What You'll Build
A player character (white square with direction indicator) that moves through tunnels using arrow keys, with collision detection preventing movement through walls.

## Key Concepts

### 1. Structs - Creating Game Entities
```c
typedef struct {
    int col;              // Grid column position
    int row;              // Grid row position
    Direction facing;     // Which way player is facing
    bool is_alive;        // For game over state (later)
} Player;
```

**What is a struct?**
- Groups related data together
- Like creating your own custom type
- Every game entity (player, enemy, powerup) uses structs

**Why structs?**

❌ Without structs:
```c
int player_col;
int player_row;
Direction player_facing;
bool player_alive;
// Hard to pass around, easy to forget variables
```

✅ With structs:
```c
Player player;
// One variable holds everything!
// Easy to pass to functions
```

### 2. Direction Enum
```c
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;
```

**Why use an enum?**
- Better than magic numbers (0, 1, 2, 3)
- Self-documenting code
- Can't accidentally use invalid value
- IDE autocomplete works
```c
// ❌ Bad
player.facing = 2;  // What does 2 mean?

// ✅ Good
player.facing = DIR_RIGHT;  // Crystal clear!
```

### 3. Initializing the Player
```c
void player_init(Player* player, int start_col, int start_row) {
    player->col = start_col;
    player->row = start_row;
    player->facing = DIR_RIGHT;  // Start facing right
    player->is_alive = true;
}
```

**Understanding the `->` operator:**
```c
Player player;
player.col = 10;        // Using . (direct access)

Player* ptr = &player;
ptr->col = 10;          // Using -> (pointer access)
// Same as: (*ptr).col = 10;
```

**Why pass pointer to function?**
```c
void player_init(Player* player, ...) {
    player->col = 10;  // Modifies actual player
}

// ❌ Without pointer (copy):
void player_init(Player player, ...) {
    player.col = 10;  // Modifies COPY, original unchanged!
}
```

### 4. Collision Detection
```c
static bool can_move_to(TileType tile) {
    // Player can walk through empty space and tunnels
    // But NOT through dirt or rocks
    return (tile == TILE_EMPTY || tile == TILE_TUNNEL);
}
```

**The `static` keyword:**
- Function only visible in this file
- Helper function, not part of public interface
- Not declared in player.h

**Collision logic:**
- TILE_EMPTY → Can move (open space)
- TILE_TUNNEL → Can move (dug path)
- TILE_DIRT → Blocked (solid)
- TILE_ROCK → Blocked (solid)

### 5. Movement with Collision
```c
bool player_move(Player* player, Direction dir, 
                 TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    int new_col = player->col;
    int new_row = player->row;
    
    // Calculate new position based on direction
    switch(dir) {
        case DIR_UP:
            new_row--;
            break;
        case DIR_DOWN:
            new_row++;
            break;
        case DIR_LEFT:
            new_col--;
            break;
        case DIR_RIGHT:
            new_col++;
            break;
    }
    
    // Check bounds
    if (new_col < 0 || new_col >= GRID_WIDTH || 
        new_row < 0 || new_row >= GRID_HEIGHT) {
        return false;  // Out of bounds
    }
    
    // Check if destination tile is walkable
    TileType destination = grid_get_tile(grid, new_row, new_col);
    if (!can_move_to(destination)) {
        return false;  // Blocked
    }
    
    // Move is valid - update position
    player->col = new_col;
    player->row = new_row;
    player->facing = dir;
    
    return true;  // Move succeeded
}
```

**Movement flow:**
```
1. Calculate new position
2. Check if in bounds
3. Check if tile is walkable
4. If both pass: update position
5. If either fails: stay in place
```

**Why check BEFORE moving?**
```c
// ❌ BAD: Move then check
player->col++;
if (blocked) {
    player->col--;  // Undo - messy!
}

// ✅ GOOD: Check then move
if (!blocked) {
    player->col++;  // Only move if valid
}
```

### 6. Keyboard Input Handling
```c
// In main game loop
while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch(event.key.key) {
            case SDLK_UP:
                player_move(&player, DIR_UP, grid);
                break;
            case SDLK_DOWN:
                player_move(&player, DIR_DOWN, grid);
                break;
            case SDLK_LEFT:
                player_move(&player, DIR_LEFT, grid);
                break;
            case SDLK_RIGHT:
                player_move(&player, DIR_RIGHT, grid);
                break;
        }
    }
}
```

**Input flow:**
```
User presses arrow key
   ↓
SDL generates KEY_DOWN event
   ↓
Event loop catches it
   ↓
Calls player_move() with direction
   ↓
Player position updates (if valid)
```

### 7. Rendering the Player
```c
void render_draw_player(SDL_Renderer* renderer, Player* player) {
    SDL_FRect player_rect;
    
    // Convert grid position to pixel position
    int x, y;
    player_get_pixel_pos(player, &x, &y);
    
    player_rect.x = x;
    player_rect.y = y;
    player_rect.w = TILE_SIZE;
    player_rect.h = TILE_SIZE;
    
    // Draw player as white square
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player_rect);
    
    // Draw direction indicator (cyan)
    SDL_FRect indicator;
    indicator.w = TILE_SIZE / 3;
    indicator.h = TILE_SIZE / 3;
    
    // Position based on facing direction
    switch(player->facing) {
        case DIR_UP:
            indicator.x = x + TILE_SIZE / 3;
            indicator.y = y + 2;
            break;
        case DIR_DOWN:
            indicator.x = x + TILE_SIZE / 3;
            indicator.y = y + TILE_SIZE - 2 - TILE_SIZE / 3;
            break;
        case DIR_LEFT:
            indicator.x = x + 2;
            indicator.y = y + TILE_SIZE / 3;
            break;
        case DIR_RIGHT:
            indicator.x = x + TILE_SIZE - 2 - TILE_SIZE / 3;
            indicator.y = y + TILE_SIZE / 3;
            break;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &indicator);
}
```

**Direction indicator explained:**
```
Facing RIGHT:
┌──────────────────────────┐
│                          │
│         ┌────┐          │  ← Small cyan square
│  WHITE  │CYAN│          │     on right side
│         └────┘          │
│                          │
└──────────────────────────┘

Facing UP:
┌──────────────────────────┐
│      ┌────┐              │  ← Cyan square
│      │CYAN│              │     near top
│      └────┘              │
│        WHITE             │
└──────────────────────────┘
```

**The math:**
```c
// For DIR_RIGHT:
indicator.x = x + TILE_SIZE - 2 - TILE_SIZE / 3;
            = x + 32 - 2 - 10
            = x + 20
// Positions indicator 20 pixels from left edge
// (near right side of tile)
```

### 8. Grid to Pixel Conversion
```c
void player_get_pixel_pos(Player* player, int* x, int* y) {
    *x = player->col * TILE_SIZE;
    *y = player->row * TILE_SIZE;
}
```

**Example conversions (TILE_SIZE = 32):**
- Grid (0, 0) → Pixel (0, 0) - top-left
- Grid (5, 3) → Pixel (160, 96)
- Grid (10, 7) → Pixel (320, 224)
- Grid (19, 14) → Pixel (608, 448) - bottom-right

**Reverse (pixel to grid):**
```c
col = pixel_x / TILE_SIZE;
row = pixel_y / TILE_SIZE;
```

## Complete Integration

### player.h
```c
#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

typedef struct {
    int col;
    int row;
    Direction facing;
    bool is_alive;
} Player;

void player_init(Player* player, int start_col, int start_row);
bool player_move(Player* player, Direction dir, 
                 TileType grid[GRID_HEIGHT][GRID_WIDTH]);
void player_get_pixel_pos(Player* player, int* x, int* y);

#endif // PLAYER_H
```

### player.c
```c
#include "player.h"
#include "grid.h"

void player_init(Player* player, int start_col, int start_row) {
    player->col = start_col;
    player->row = start_row;
    player->facing = DIR_RIGHT;
    player->is_alive = true;
}

static bool can_move_to(TileType tile) {
    return (tile == TILE_EMPTY || tile == TILE_TUNNEL);
}

bool player_move(Player* player, Direction dir, 
                 TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    int new_col = player->col;
    int new_row = player->row;
    
    switch(dir) {
        case DIR_UP:    new_row--; break;
        case DIR_DOWN:  new_row++; break;
        case DIR_LEFT:  new_col--; break;
        case DIR_RIGHT: new_col++; break;
    }
    
    if (new_col < 0 || new_col >= GRID_WIDTH || 
        new_row < 0 || new_row >= GRID_HEIGHT) {
        return false;
    }
    
    TileType destination = grid_get_tile(grid, new_row, new_col);
    if (!can_move_to(destination)) {
        return false;
    }
    
    player->col = new_col;
    player->row = new_row;
    player->facing = dir;
    
    return true;
}

void player_get_pixel_pos(Player* player, int* x, int* y) {
    *x = player->col * TILE_SIZE;
    *y = player->row * TILE_SIZE;
}
```

### Updated main.c
```c
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "types.h"
#include "grid.h"
#include "render.h"
#include "player.h"

int main(void) {
    // [SDL initialization]
    
    // Create grid
    TileType grid[GRID_HEIGHT][GRID_WIDTH];
    grid_init(grid);
    
    // Create player
    Player player;
    player_init(&player, 10, 2);  // Start in tunnel
    
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
                else if (event.key.key == SDLK_UP) {
                    player_move(&player, DIR_UP, grid);
                }
                else if (event.key.key == SDLK_DOWN) {
                    player_move(&player, DIR_DOWN, grid);
                }
                else if (event.key.key == SDLK_LEFT) {
                    player_move(&player, DIR_LEFT, grid);
                }
                else if (event.key.key == SDLK_RIGHT) {
                    player_move(&player, DIR_RIGHT, grid);
                }
            }
        }
        
        // Render
        SDL_RenderClear(renderer);
        render_draw_grid(renderer, grid);
        render_draw_player(renderer, &player);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16);
    }
    
    // [Cleanup]
    return 0;
}
```

### Updated Makefile
```makefile
SOURCES = main.c grid.c render.c player.c
OBJECTS = main.o grid.o render.o player.o
HEADERS = types.h grid.h render.h player.h

$(TARGET): $(OBJECTS)
	gcc $(OBJECTS) -lSDL3 -o $(TARGET)

main.o: main.c $(HEADERS)
	gcc -c main.c -o main.o

grid.o: grid.c types.h grid.h
	gcc -c grid.c -o grid.o

render.o: render.c types.h render.h player.h
	gcc -c render.c -o render.o

player.o: player.c types.h player.h grid.h
	gcc -c player.c -o player.o
```

## Experiments

1. **Add diagonal movement**
```c
   case SDLK_KP_7:  // Numpad 7 (up-left)
       player_move(&player, DIR_UP, grid);
       player_move(&player, DIR_LEFT, grid);
       break;
```

2. **Add player speed control**
```c
   // In player.h
   typedef struct {
       int col, row;
       Direction facing;
       bool is_alive;
       int speed;  // Tiles per second
   } Player;
```

3. **Track player position history**
```c
   #define MAX_HISTORY 100
   typedef struct {
       int col, row;
   } Position;
   
   Position history[MAX_HISTORY];
   int history_count = 0;
```

4. **Add animation state**
```c
   typedef enum {
       ANIM_IDLE,
       ANIM_WALKING,
       ANIM_DIGGING
   } AnimState;
   
   typedef struct {
       // ... existing fields
       AnimState anim_state;
       int anim_frame;
   } Player;
```

## Common Mistakes

❌ **Forgetting to pass grid to player_move**
```c
player_move(&player, DIR_UP);  // Can't check collision!
```

❌ **Moving without checking**
```c
player->col++;  // Might walk through wall!
```

❌ **Wrong pointer syntax**
```c
Player player;
player_init(player, 10, 2);  // WRONG - need &player
```

❌ **Bounds checking after moving**
```c
player->col++;
if (player->col >= GRID_WIDTH) {
    player->col--;  // Messy undo
}
```

❌ **Not updating facing direction**
```c
// Player moves but still faces old direction
// Looks wrong visually
```

## Understanding Pointers with Structs

**Dot vs Arrow:**
```c
Player player;        // Actual struct
player.col = 5;       // Use . to access members

Player* ptr = &player;  // Pointer to struct
ptr->col = 5;           // Use -> to access members
(*ptr).col = 5;         // Same thing, uglier
```

**Why functions take Player*:**
```c
void update_player(Player player) {
    player.col = 10;
    // Changes COPY, original unchanged
}

void update_player(Player* player) {
    player->col = 10;
    // Changes ORIGINAL
}
```

**Passing to functions:**
```c
Player player;
update_player(&player);  // Pass address with &
```

## Key Takeaways

✅ Structs group related entity data  
✅ Enums make direction code readable  
✅ Check collision BEFORE moving  
✅ Pointers let functions modify data  
✅ Arrow operator (->) accesses pointer members  
✅ Static functions are file-private helpers  
✅ Input → Logic → Render pattern  
✅ Grid coords convert to pixels for drawing  

## Next Step

**Step 5**: Add digging mechanics - turn TILE_DIRT into TILE_TUNNEL as you move!
