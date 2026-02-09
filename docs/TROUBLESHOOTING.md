# Troubleshooting Guide

## Compilation Errors

### "undefined reference to" Error

**Symptom:**
```
/usr/bin/ld: main.o: undefined reference to `grid_init'
/usr/bin/ld: main.o: undefined reference to `render_draw_grid'
```

**Cause:**
- Forgot to compile .c files
- Forgot to link object files
- Function declared but not implemented

**Solution 1: Missing source files in compilation**
```bash
# ❌ Wrong - only compiling main.c
gcc main.c -lSDL3 -o digdug

# ✅ Right - compile all source files
gcc main.c grid.c render.c player.c -lSDL3 -o digdug
```

**Solution 2: Check Makefile**
```makefile
# Make sure all .c files are listed
SOURCES = main.c grid.c render.c player.c
```

**Solution 3: Missing function implementation**
```c
// player.h declares it
void player_init(Player* p, int col, int row);

// player.c MUST implement it
void player_init(Player* p, int col, int row) {
    // Implementation here
}
```

### "SDL3 not found" Error

**Symptom:**
```
fatal error: SDL3/SDL.h: No such file or directory
```

**Cause:**
- SDL3 not installed
- Compiler can't find SDL3 headers

**Solution:**
```bash
# Check if SDL3 is installed
pacman -Q sdl3
# or
pkg-config --modversion sdl3

# If not installed
sudo pacman -S sdl3

# If installed but not found, add include path
gcc -I/usr/include/SDL3 main.c ...
```

### "multiple definition" Error

**Symptom:**
```
multiple definition of `TILE_SIZE'
```

**Cause:**
- #define in header without header guards
- Variable definition in header (should be declaration)

**Solution 1: Add header guards**
```c
// types.h
#ifndef TYPES_H
#define TYPES_H

#define TILE_SIZE 32

#endif
```

**Solution 2: Use const in .c file**
```c
// ❌ Wrong - in header
#define GRID_WIDTH 20

// ✅ Right - in one .c file
// types.c
const int GRID_WIDTH = 20;

// types.h (just declare)
extern const int GRID_WIDTH;
```

### Integer Division Warning

**Symptom:**
```
warning: conversion from 'int' to 'float' may change value
```

**Code:**
```c
SDL_FRect rect;
rect.w = TILE_SIZE / 3;  // 32 / 3 = 10.666... → 10
```

**Solution 1: Pre-calculate constants**
```c
#define TILE_SIZE 32
#define TILE_INDICATOR_SIZE 10  // TILE_SIZE / 3
#define TILE_INDICATOR_MARGIN 2

rect.w = TILE_INDICATOR_SIZE;
```

**Solution 2: Explicit cast**
```c
rect.w = (float)(TILE_SIZE / 3);
```

**Solution 3: Float division**
```c
rect.w = TILE_SIZE / 3.0f;
```

### Unused Parameter Warning

**Symptom:**
```
warning: unused parameter 'argc' [-Wunused-parameter]
int main(int argc, char *argv[]) {
```

**Solution 1: Remove if not needed**
```c
int main(void) {
    // No parameters
}
```

**Solution 2: Mark as unused**
```c
int main(int argc, char *argv[]) {
    (void)argc;  // Explicitly unused
    (void)argv;
}
```

**Solution 3: Disable warning (not recommended)**
```bash
gcc -Wno-unused-parameter main.c
```

## Runtime Errors

### Segmentation Fault

**Common causes:**

**1. NULL pointer dereference**
```c
Player* player = NULL;
player->col = 5;  // CRASH!

// Fix: Check for NULL
if (player != NULL) {
    player->col = 5;
}
```

**2. Array out of bounds**
```c
int grid[20][15];
grid[20][15] = TILE_DIRT;  // CRASH! (indices start at 0)

// Fix: Bounds checking
if (row >= 0 && row < GRID_HEIGHT &&
    col >= 0 && col < GRID_WIDTH) {
    grid[row][col] = TILE_DIRT;
}
```

**3. Stack overflow (large array)**
```c
int huge_grid[10000][10000];  // CRASH! Too big for stack

// Fix: Use malloc or make global
int* huge_grid = malloc(10000 * 10000 * sizeof(int));
```

**4. Uninitialized pointer**
```c
Player* player;  // Uninitialized
player->col = 5;  // CRASH!

// Fix: Initialize
Player* player = malloc(sizeof(Player));
// or
Player player;  // Stack allocation
player.col = 5;
```

### Black Screen / Nothing Renders

**Cause 1: Forgot SDL_RenderPresent()**
```c
// ❌ Wrong
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
// Nothing shows!

// ✅ Right
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
SDL_RenderPresent(renderer);  // THIS!
```

**Cause 2: Drawing outside screen**
```c
// If screen is 640x480
rect.x = 1000;  // Off screen!
rect.y = 2000;  // Off screen!
```

**Cause 3: Wrong color (black on black)**
```c
// Background is black
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
SDL_RenderClear(renderer);

// Drawing black rectangles - invisible!
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
SDL_RenderFillRect(renderer, &rect);
```

### Player Can't Move

**Cause 1: Collision detection too strict**
```c
// Check what can_move_to returns
printf("Tile type: %d, Can move: %d\n", 
       tile, can_move_to(tile));
```

**Cause 2: Wrong grid initialization**
```c
// Check grid contents
for (int row = 0; row < GRID_HEIGHT; row++) {
    for (int col = 0; col < GRID_WIDTH; col++) {
        printf("%d ", grid[row][col]);
    }
    printf("\n");
}
```

**Cause 3: Player starts in solid tile**
```c
// Make sure starting position is valid
player_init(&player, 10, 2);  // Is grid[2][10] walkable?
```

### Player Moves Too Fast

**Cause: No frame rate control**
```c
// ❌ Wrong - no delay
while (running) {
    handle_input();
    update();
    render();
}

// ✅ Right - 60 FPS
while (running) {
    handle_input();
    update();
    render();
    SDL_Delay(16);  // ~60 FPS
}
```

## Git Issues

### Merge Conflict

**Symptom:**
```
CONFLICT (content): Merge conflict in README.md
```

**Solution:**
```bash
# Open the file, look for conflict markers
<<<<<<< HEAD
⬜ **Step 4**: Player character
=======
✅ **Step 4**: Player character
>>>>>>> commit_hash

# Keep what you want, remove markers
✅ **Step 4**: Player character

# Mark as resolved
git add README.md
git rebase --continue
```

### Diverged Branches

**Symptom:**
```
Your branch and 'origin/main' have diverged
```

**Solution 1: Rebase (clean history)**
```bash
git pull --rebase origin main
# Resolve conflicts if any
git push origin main
```

**Solution 2: Merge (preserves both)**
```bash
git pull origin main
# Resolve conflicts if any
git push origin main
```

**Solution 3: Force push (dangerous!)**
```bash
# Only if you're sure local is correct
git push --force-with-lease origin main
```

### Accidentally Committed Build Artifacts

**Solution:**
```bash
# Remove from git but keep locally
git rm --cached *.o
git rm --cached digdug

# Add to .gitignore
echo "*.o" >> .gitignore
echo "digdug" >> .gitignore

# Commit
git add .gitignore
git commit -m "Remove build artifacts and update .gitignore"
```

## SDL3 Issues

### Window Won't Close

**Cause: Not handling SDL_EVENT_QUIT**
```c
// ❌ Wrong
while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        // Handle keys
    }
    // Forgot to check QUIT!
}

// ✅ Right
while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
        running = false;
    }
    else if (event.type == SDL_EVENT_KEY_DOWN) {
        // Handle keys
    }
}
```

### Key Input Not Working

**Cause 1: Wrong event type**
```c
// ❌ Wrong
if (event.type == SDL_EVENT_KEY_UP) {  // Release, not press!

// ✅ Right
if (event.type == SDL_EVENT_KEY_DOWN) {
```

**Cause 2: Wrong key constant**
```c
// ❌ Wrong (SDL2 style)
if (event.key.keysym.sym == SDLK_UP) {

// ✅ Right (SDL3 style)
if (event.key.key == SDLK_UP) {
```

**Cause 3: Not polling events**
```c
// ❌ Wrong - single poll
SDL_PollEvent(&event);

// ✅ Right - loop until no more events
while (SDL_PollEvent(&event)) {
    // Handle event
}
```

### Memory Leak

**Symptom:**
```
Memory usage grows over time
Program crashes after running for a while
```

**Common causes:**

**1. Not destroying SDL objects**
```c
// ❌ Wrong
SDL_Quit();
// Forgot to destroy window and renderer!

// ✅ Right
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
```

**2. Creating objects in loop**
```c
// ❌ Wrong
while (running) {
    SDL_Texture* tex = SDL_CreateTexture(...);
    // Use tex
    // Forgot to destroy!
}

// ✅ Right
SDL_Texture* tex = SDL_CreateTexture(...);
while (running) {
    // Use tex
}
SDL_DestroyTexture(tex);
```

## Logic Errors

### Player Walks Through Walls

**Debug:**
```c
bool player_move(...) {
    TileType dest = grid_get_tile(grid, new_row, new_col);
    printf("Moving to (%d,%d), tile=%d, can_move=%d\n",
           new_col, new_row, dest, can_move_to(dest));
    
    if (!can_move_to(dest)) {
        return false;
    }
    // ...
}
```

**Possible issues:**
- can_move_to() logic wrong
- Grid not initialized correctly
- Passing wrong coordinates

### Drawing in Wrong Position

**Debug:**
```c
void render_draw_player(...) {
    int x, y;
    player_get_pixel_pos(player, &x, &y);
    printf("Drawing player at pixel (%d,%d), grid (%d,%d)\n",
           x, y, player->col, player->row);
    // ...
}
```

**Check:**
- Conversion formula correct?
- Grid position within bounds?
- TILE_SIZE defined correctly?

### Collision Detection Feels Wrong

**Common issue: Off-by-one errors**
```c
// ❌ Wrong
if (new_col < 0 || new_col > GRID_WIDTH) {  // Should be >=

// ✅ Right
if (new_col < 0 || new_col >= GRID_WIDTH) {
```

## Performance Issues

### Game Runs Slowly

**Cause 1: No delay (CPU maxed)**
```c
// Add frame rate control
SDL_Delay(16);
```

**Cause 2: Rendering too much**
```c
// Only render what changed
// Use dirty rectangles
// Render to texture once, reuse
```

**Cause 3: Debug build**
```bash
# Compile with optimizations
gcc -O2 main.c ...
```

### Game Runs Too Fast

**Cause: Delay too short or missing**
```c
// Too fast
SDL_Delay(1);

// 60 FPS
SDL_Delay(16);

// 30 FPS
SDL_Delay(33);
```

## Debugging Checklist

When something doesn't work:

1. **Read the error message carefully**
   - What file?
   - What line?
   - What's the actual error?

2. **Add printf debugging**
```c
   printf("DEBUG: Reached here\n");
   printf("DEBUG: x=%d, y=%d\n", x, y);
```

3. **Check your assumptions**
   - Is the variable what you think?
   - Is the function being called?
   - Is the condition true/false?

4. **Simplify**
   - Comment out code
   - Test small parts
   - Isolate the problem

5. **Use the compiler**
```bash
   # Enable all warnings
   gcc -Wall -Wextra -Wpedantic main.c
```

6. **Use a debugger**
```bash
   gcc -g main.c -o main
   gdb ./main
```

7. **Check the docs**
   - SDL3 wiki
   - C reference
   - Man pages

8. **Take a break**
   - Fresh eyes find bugs faster
   - Rubber duck debugging works!

## Getting Help

**Before asking for help, provide:**

1. **Exact error message**
```
   main.c:42:5: error: 'SDL_Window' undeclared
```

2. **Relevant code**
```c
   // The function that's failing
```

3. **What you tried**
   - "I checked SDL3 is installed"
   - "I added the header guards"
   - "I verified the file exists"

4. **Your environment**
   - OS: Arch Linux
   - Compiler: GCC 13.2
   - SDL3 version: 3.4.0

**Good question format:**
```
Problem: Player can move through dirt tiles
Expected: Should be blocked by TILE_DIRT
Code: [paste relevant code]
Debugging: Added printf, shows can_move_to returns true for TILE_DIRT
Environment: Ubuntu 24, SDL3 3.4.0, GCC 13
```

## Resources

- [SDL3 Wiki](https://wiki.libsdl.org/SDL3/)
- [C Reference](https://en.cppreference.com/)
- [GDB Tutorial](https://www.sourceware.org/gdb/documentation/)
- [Valgrind Memory Checker](https://valgrind.org/)

## Key Takeaways

✅ Read error messages carefully  
✅ Use printf debugging liberally  
✅ Check assumptions with prints  
✅ Enable compiler warnings  
✅ Test small parts in isolation  
✅ Use version control (git)  
✅ Take breaks when stuck  
✅ Search before asking  
✅ Provide context when asking for help
