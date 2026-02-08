# Dig Dug Clone - Learning SDL3 Step by Step

## About Dig Dug

**Dig Dug** is a classic arcade game from 1982 where you tunnel through underground dirt to hunt monsters. You defeat enemies by inflating them with your air pump until they explode. The game features Pookas (red balloon-like monsters) and Fygars (green fire-breathing dragons). As you dig, rocks can fall and crush enemies. The deeper you go, the more points you earn!

This project recreates Dig Dug from scratch using SDL3 and C, teaching fundamental game programming concepts step-by-step.

---

## Current Progress

✅ **Step 1**: SDL3 window and game loop  
✅ **Step 2**: Drawing rectangles (tile building blocks)  
✅ **Step 3**: Full tile grid system + modular code structure  
⬜ **Step 4**: Player character with keyboard movement  
⬜ **Step 5**: Digging mechanics (coming next!)  
⬜ **Step 5**: Digging mechanics (coming next!)  
⬜ **Step 6**: Enemies and AI  
⬜ **Step 7**: Air pump weapon  
⬜ **Step 8**: Rocks and physics  
⬜ **Step 9**: Score and game states  

---

## Step 1: Understanding the Basics

**Goal:** Create a window, understand the game loop, and handle basic events.

### What You Learned

1. **SDL3 Initialization**
   - `SDL_Init(SDL_INIT_VIDEO)` - Must be called first
   - Always check for errors with `SDL_GetError()`

2. **Core Objects**
   - `SDL_Window` - Your game window
   - `SDL_Renderer` - The thing that draws to the window
   - Think: Window = canvas, Renderer = paintbrush

3. **The Game Loop** (Most Important!)
   ```c
   while (game is running):
       1. Handle Input (keyboard, mouse, close button)
       2. Update Game State (movement, collisions)
       3. Render (draw everything)
       4. Delay (control frame rate)
   ```

4. **Event Handling**
   - `SDL_PollEvent()` - Check for events in a loop
   - `SDL_EVENT_QUIT` - User closed window
   - `SDL_EVENT_KEY_DOWN` - Key pressed

5. **Basic Rendering**
   - `SDL_SetRenderDrawColor()` - Choose a color
   - `SDL_RenderClear()` - Fill screen with color
   - `SDL_RenderPresent()` - Show what you drew (like flipping pages)

### Key Concepts

**Frame Rate Control:**
- `SDL_Delay(16)` ≈ 60 FPS (1000ms / 60 = 16.67ms)
- Too high = slow, too low = CPU maxed

**Cleanup Order:**
- Destroy in REVERSE order of creation
- Renderer before Window before SDL_Quit()

---

## Step 2: Drawing Rectangles (Tile Basics!)

**Goal:** Learn to draw rectangles - the building blocks of all tiles.

### What You Learned

1. **SDL_FRect Structure**
   ```c
   SDL_FRect rect = {
       .x = 100,    // Position from left edge
       .y = 100,    // Position from top edge
       .w = 32,     // Width in pixels
       .h = 32      // Height in pixels
   };
   ```
   - This defines ONE rectangle (future: ONE tile)
   - `SDL_FRect` uses floats for smooth movement

2. **SDL_RenderFillRect()**
   - Draws a filled rectangle
   - Must set color BEFORE calling this
   ```c
   SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red
   SDL_RenderFillRect(renderer, &rect);               // Draw it
   ```

3. **Drawing Multiple Shapes**
   - Use arrays to store multiple rectangles
   - Use loops to draw them efficiently
   - This is how we'll make our tile grid!

4. **RGB Colors**
   - RGB = Red, Green, Blue (0-255 each)
   - Brown dirt: (139, 69, 19)
   - Gray rock: (128, 128, 128)

### Key Insight

Drawing order matters! Things drawn first appear BEHIND later things, like painting layers.

---

## Step 3: Full Tile Grid System & Modular Code

**Goal:** Create a complete tile-based world and organize code into modules.

### Code Structure

The code is now split into **modules** - this is how real games are organized!

```
digdug/
├── main.c           - Game loop and SDL initialization
├── types.h          - Shared types and constants
├── grid.h/grid.c    - Grid logic (init, get, set tiles)
├── render.h/render.c - All rendering code
├── player.h/player.c - Player logic and movement
├── Makefile         - Builds everything together
└── README.md        - This file!
```

### What You Learned

**1. 2D Arrays - The Game Map**
```c
TileType grid[GRID_HEIGHT][GRID_WIDTH];
// grid[row][column] = grid[y][x]
```
- This is your entire game world in memory (20×15 = 300 tiles)
- Each cell holds a tile type
- Think of it like a spreadsheet

**2. Enums - Tile Types**
```c
typedef enum {
    TILE_EMPTY = 0,   // Black - nothing there
    TILE_DIRT,        // Brown - solid dirt
    TILE_TUNNEL,      // Dark brown - dug out path
    TILE_ROCK         // Gray - falling rock
} TileType;
```
- Better than magic numbers (0, 1, 2, 3)
- Makes code readable
- Easy to add new types

**3. Nested Loops - Drawing Everything**
```c
for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
        // Draw tile at grid[row][col]
    }
}
```
- Outer loop = rows (top to bottom)
- Inner loop = columns (left to right)
- This pattern is used in EVERY tile-based game!

**4. Grid-to-Pixel Conversion**
```c
pixel_x = col * TILE_SIZE;  // Column 5 → pixel 160
pixel_y = row * TILE_SIZE;  // Row 3 → pixel 96
```

**5. Modular Architecture**
- **types.h** - Shared definitions everyone needs
- **grid.c** - World logic (no rendering!)
- **render.c** - Drawing logic (no game rules!)
- **main.c** - Coordinates everything

### Header Guards - Important!

Every .h file has:
```c
#ifndef TYPES_H
#define TYPES_H
// ... code ...
#endif
```

**Why?** Prevents including the same file twice and getting compiler errors.

### Compilation Process

```
1. main.c    → main.o    (includes types.h, grid.h, render.h, player.h)
2. grid.c    → grid.o    (includes types.h, grid.h)
3. render.c  → render.o  (includes types.h, render.h, player.h)
4. player.c  → player.o  (includes types.h, player.h, grid.h)
5. Link all .o files → digdug executable
```

If you change **types.h**, ALL files recompile.
If you change **render.c**, only render.c recompiles.

### Key Programming Concepts

**Separation of Concerns:**
- Grid module: "What is the world?"
- Render module: "How do we show it?"
- Player module: "How does the player behave?"
- Main: "How does it all work together?"

**Single Responsibility:**
- Each .c file has ONE job
- Don't mix logic and rendering
- Makes code easier to understand and modify

**Interface vs Implementation:**
- `.h` files = "What can I do?" (declarations)
- `.c` files = "How do I do it?" (definitions)

---

## Step 4: Player Character & Movement

**Goal:** Add a controllable character with keyboard movement and collision detection.

### What You Learned

**1. Structs - Game Entities**
```c
typedef struct {
    int col;              // Grid position X
    int row;              // Grid position Y
    Direction facing;     // Which way player faces
    bool is_alive;        // Game state
} Player;
```
- Structs group related data together
- Like creating your own custom type
- Every game entity uses structs

**2. Enums for Direction**
```c
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;
```

**3. Collision Detection**
```c
bool can_move_to(TileType tile) {
    return (tile == TILE_EMPTY || tile == TILE_TUNNEL);
}
```
- Check BEFORE moving
- Prevents walking through walls
- Returns true/false for valid moves

**4. Grid-Based Movement**
- Player stores grid position (col, row)
- Movement = change col or row by 1
- Convert to pixels for rendering

**5. Input Handling for Gameplay**
```c
if (event.key.key == SDLK_UP) {
    player_move(&player, DIR_UP, grid);
}
```
- Arrow keys mapped to directions
- Each keypress attempts a move
- Move fails if blocked

### Player Movement Flow

```
1. User presses arrow key
2. Calculate new position (col±1, row±1)
3. Check if new position is in bounds
4. Check if destination tile is walkable
5. If valid: update player position
6. If blocked: stay in place
```

### Player Rendering

The player is drawn as:
- **White square** - the main body
- **Cyan indicator** - shows which direction facing
- Later we'll replace with actual sprites!

### Controls

- **Arrow Keys**: Move Dig Dug
- **ESC**: Quit

---

## Building and Running

```bash
# Clean old builds
make clean

# Compile everything
make

# Run the game
./digdug

# Or compile and run in one command
make run
```

### Manual Compilation (if needed)

```bash
# Compile each source file
gcc -Wall -Wextra -std=c11 -g -c main.c -o main.o
gcc -Wall -Wextra -std=c11 -g -c grid.c -o grid.o
gcc -Wall -Wextra -std=c11 -g -c render.c -o render.o
gcc -Wall -Wextra -std=c11 -g -c player.c -o player.o

# Link everything together
gcc main.o grid.o render.o player.o -lSDL3 -o digdug
```

---

## Experiments to Try

1. **Add a new tile type:**
   - Edit `types.h`: Add `TILE_WATER` to enum
   - Edit `render.c`: Add color case (0, 100, 255)
   - Edit `grid.c`: Place some water tiles
   - Update collision: Can player walk through water?

2. **Change player speed:**
   - In `main.c`, change `SDL_Delay(16)` to `SDL_Delay(33)`
   - Makes game run at 30 FPS instead of 60 FPS

3. **Modify starting level:**
   - Edit `grid_init()` in `grid.c`
   - Create different tunnel patterns
   - Place rocks in different locations

4. **Add debug output:**
   - In `player_move()`, add:
   ```c
   printf("Player at (%d, %d)\n", player->col, player->row);
   ```

5. **Change colors:**
   - Edit `render_get_tile_color()` to customize tile colors
   - Edit `render_draw_player()` to change player color

---

## Key Questions to Understand

1. Why do we use `grid[row][col]` instead of `grid[col][row]`?
2. What's the difference between grid coordinates and pixel coordinates?
3. Why do we check collision BEFORE moving the player?
4. What happens if you remove bounds checking in `player_move()`?
5. Why does main.c include multiple header files?
6. What does the `static` keyword mean in `can_move_to()`?

---

## Common Patterns in Game Programming

**Entity Management:**
- Every game object (player, enemy, bullet) has position, state, behavior
- Structs hold entity data
- Functions operate on entities

**Input → Logic → Render Loop:**
- Input: User pressed key
- Logic: Update player position
- Render: Draw player at new position
- Repeats 60 times per second!

**Collision Before Movement:**
- Always check if move is valid FIRST
- Only update position if valid
- Prevents bugs and glitches

**Grid-Based Movement:**
- Position stored as grid coordinates
- Convert to pixels for rendering
- Simple and predictable

---

## What's Next - Step 5: Digging Mechanics!

Now that you can move, let's add the core Dig Dug gameplay:

- **Dig through dirt** - Turn TILE_DIRT into TILE_TUNNEL as you move
- **Visual feedback** - See the path you carve
- **Underground exploration** - Create your own tunnels
- **Score tracking** - Points for dirt removed

This is where it becomes the actual Dig Dug experience!

---

## Resources

- **SDL3 Documentation**: https://wiki.libsdl.org/SDL3/
- **Original Dig Dug**: https://en.wikipedia.org/wiki/Dig_Dug
- **C Programming**: https://en.cppreference.com/

---

## Learning Objectives Summary

✅ SDL3 initialization and cleanup  
✅ Game loop architecture  
✅ Event handling (keyboard, window events)  
✅ 2D rendering with rectangles  
✅ Tile-based game worlds  
✅ Multi-file C projects  
✅ Header guards and includes  
✅ Structs and enums  
✅ Grid-based movement  
✅ Collision detection  
✅ Modular code organization  

**You're learning real game development patterns used in professional games!**
