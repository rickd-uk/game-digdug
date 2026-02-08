# Dig Dug Clone - Learning SDL3 Step by Step

## Step 3: Modular Code Structure

### What You're Learning - Code Organization!

The code is now split into **modules** - this is how real games are organized!

**File Structure:**
```
digdug/
├── main.c           - Game loop and SDL initialization
├── types.h          - Shared types and constants
├── grid.h/grid.c    - Grid logic (init, get, set tiles)
├── render.h/render.c - All rendering code
├── Makefile         - Builds everything together
└── README.md        - This file!
```

### Why Split Files?

**Before (1 file):**
- Hard to find things
- Changes affect everything
- Can't reuse code easily

**After (modules):**
- ✅ Each file has ONE job
- ✅ Easy to find code
- ✅ Can reuse modules in other projects
- ✅ Multiple people can work on different parts
- ✅ Changes to render.c don't affect grid.c

### The Files Explained

**1. types.h** - Shared Definitions
```c
#define SCREEN_WIDTH 640    // Everyone needs these
#define TILE_SIZE 32
typedef enum { TILE_DIRT, ... } TileType;
```
- Constants used everywhere
- Type definitions (enums, structs)
- Include guards prevent double-inclusion

**2. grid.h / grid.c** - Grid Logic
```c
void grid_init(...)        // Set up initial level
TileType grid_get_tile(...) // Read a tile
void grid_set_tile(...)    // Change a tile
```
- Everything about the game world
- No rendering code here!
- Has bounds checking (safety!)

**3. render.h / render.c** - Drawing
```c
void render_get_tile_color(...)  // Colors for tiles
void render_draw_grid(...)        // Draw everything
```
- All SDL rendering code
- Knows about grid, but grid doesn't know about render!
- One-way dependency: render → grid

**4. main.c** - The Boss
```c
main() {
    init SDL
    create grid
    game loop:
        handle input
        update (later!)
        render
    cleanup
}
```
- Coordinates everything
- Minimal code - just the loop
- Calls functions from other modules

### Header Guards - Important Concept!

Every .h file has:
```c
#ifndef TYPES_H      // If not defined
#define TYPES_H      // Define it
// ... code ...
#endif               // End if
```

**Why?** Prevents including the same file twice:
```c
// Without guards - ERROR!
#include "types.h"  
#include "grid.h"   // grid.h also includes types.h
// TileType defined twice = compiler error!

// With guards - OK!
// First include: defines TileType
// Second include: skipped (already defined)
```

### Building Multiple Files

The Makefile handles this:
```makefile
SOURCES = main.c grid.c render.c
OBJECTS = main.o grid.o render.o

# Compiles each .c to .o
# Links all .o files together
```

### Compilation Process

```
1. main.c    → main.o    (includes types.h, grid.h, render.h)
2. grid.c    → grid.o    (includes types.h, grid.h)
3. render.c  → render.o  (includes types.h, render.h)
4. Link all .o files → digdug executable
```

If you change **types.h**, ALL files recompile.
If you change **render.c**, only render.c recompiles.

### Build and Run

```bash
make clean   # Remove old builds
make         # Compile everything
./digdug     # Run!

# Or just:
make run
```

### Experiments

1. **Add a new tile type:**
   - Edit `types.h`: Add `TILE_WATER` to enum
   - Edit `render.c`: Add color case (0, 100, 255)
   - Edit `grid.c`: Place some water tiles

2. **Add a new function:**
   - In `grid.h`: Declare `void grid_clear(TileType grid[...]);`
   - In `grid.c`: Implement it (set all to TILE_EMPTY)
   - In `main.c`: Call it before grid_init()

3. **See dependencies:**
   ```bash
   make clean
   make
   # Watch order: main.o, grid.o, render.o, then link
   ```

4. **Test header guards:**
   - Try removing `#ifndef` from types.h
   - See compiler error about redefinition
   - Put it back!

### Understanding the Flow

**Reading Code:**
1. Start with main.c
2. See `grid_init()` call? Go to grid.h to see declaration
3. Want details? Go to grid.c to see implementation
4. See `render_draw_grid()`? Follow same pattern

**Adding Features:**
1. Think: "What module does this belong in?"
2. Grid logic? → grid.c
3. Drawing? → render.c  
4. Neither? → Maybe new module?

### Key Programming Concepts

**Separation of Concerns:**
- Grid module: "What is the world?"
- Render module: "How do we show it?"
- Main: "How does it all work together?"

**Single Responsibility:**
- Each .c file has ONE job
- grid.c manages the world
- render.c draws things
- Don't mix jobs!

**Interface (header) vs Implementation (source):**
- `.h` = "What can I do?" (declarations)
- `.c` = "How do I do it?" (definitions)
- Users only need to read .h files

### Questions to Answer

1. If you change TILE_SIZE in types.h, what files recompile?
2. Can grid.c call render functions? Should it?
3. Where would you add a `grid_count_dirt()` function?
4. Why does main.c include grid.h AND render.h?
5. What happens if you forget to add a new .c file to the Makefile?

## What's Next

Now that the code is organized, we can add features easily:

- **Step 4**: Input handling module (keyboard/mouse)
- **Step 5**: Player module (character, movement)
- **Step 6**: Game logic module (digging, collisions)

Clean code makes adding features easy!

This structure is used in REAL game engines. You're learning industry practices!
