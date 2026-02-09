# Troubleshooting Guide

## Compilation Errors

### "undefined reference to" Error

**Symptom:**
```
/usr/bin/ld: main.o: undefined reference to `grid_init'
/usr/bin/ld: main.o: undefined reference to `render_draw_grid'
/usr/bin/ld: main.o: undefined reference to `player_update'
```

**Cause:**
- Forgot to compile .c files
- Forgot to link object files
- Function declared but not implemented

**Solution 1: Missing source files in compilation**
```bash
# âŒ Wrong - only compiling main.c
gcc main.c -lSDL3 -o digdug

# âœ… Right - compile all source files
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
// âŒ Wrong - in header
#define GRID_WIDTH 20

// âœ… Right - in one .c file
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
rect.w = TILE_SIZE / 3;  // 32 / 3 = 10.666... â†’ 10
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
// âŒ Wrong
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
// Nothing shows!

// âœ… Right
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

**Cause 4: Cooldown stuck (Step 5)**
```c
// Debug the slowdown system
printf("move_slowdown: %d\n", player.move_slowdown);

// Make sure player_update() is being called
void player_update(Player* player) {
    printf("Updating, slowdown: %d\n", player->move_slowdown);
    if (player->move_slowdown > 0) {
        player->move_slowdown--;
    }
}

// Check it's in main loop
while (running) {
    handle_input(&event, &running, &player, grid);
    player_update(&player);  // THIS MUST BE CALLED!
    // render...
}
```

### Player Moves Too Fast

**Cause: No frame rate control**
```c
// âŒ Wrong - no delay
while (running) {
    handle_input();
    update();
    render();
}

// âœ… Right - 60 FPS
while (running) {
    handle_input();
    update();
    render();
    SDL_Delay(16);  // ~60 FPS
}
```

### Player Moves Too Slowly (Step 5)

**Symptom:**
Player barely moves or takes forever to respond to input.

**Cause 1: Slowdown value too high**
```c
// âŒ Wrong - way too slow
player->move_slowdown = 120;  // 2 seconds!

// âœ… Right - feels good
player->move_slowdown = 8;   // Digging
player->move_slowdown = 3;   // Running
```

**Cause 2: Slowdown not decrementing**
```c
// Make sure player_update() is called every frame
printf("Before update: %d\n", player.move_slowdown);
player_update(&player);
printf("After update: %d\n", player.move_slowdown);
// Should decrease by 1 each time
```

**Cause 3: Slowdown initialized wrong**
```c
// âŒ Wrong
void player_init(Player* player, int col, int row) {
    player->move_slowdown = 10;  // Starts frozen!
}

// âœ… Right
void player_init(Player* player, int col, int row) {
    player->move_slowdown = 0;  // Can move immediately
}
```

### Player Moves Instantly Through Dirt (Step 5)

**Symptom:**
No speed difference between digging and running.

**Cause: Not setting slowdown after digging**
```c
// âŒ Wrong - no slowdown applied
if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    // Forgot to set slowdown!
}

// âœ… Right - different speeds
bool just_dug = false;
if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    just_dug = true;
}

if (just_dug) {
    player->move_slowdown = 8;  // Slow for digging
} else {
    player->move_slowdown = 3;  // Fast for tunnels
}
```

### Player Can Dig Upward (Step 5)

**Symptom:**
Player digs through ceiling, violating gravity rules.

**Cause: Missing or wrong order of checks**
```c
// âŒ Wrong - checks after converting to tunnel
if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    destination = TILE_TUNNEL;
}
// Too late to check direction now!
if (destination == TILE_DIRT && dir == DIR_UP) {
    return false;
}

// âœ… Right - check BEFORE converting
TileType destination = grid_get_tile(grid, new_row, new_col);

// Block upward digging FIRST
if (destination == TILE_DIRT && dir == DIR_UP) {
    return false;
}

// Then do digging
if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    destination = TILE_TUNNEL;
}
```

### Dirt Counter Not Increasing (Step 5)

**Symptom:**
HUD bars don't appear even though player is digging.

**Cause 1: Not incrementing counter**
```c
// âŒ Wrong
if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    // Forgot to increment!
}

// âœ… Right
if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    player->dirt_dug++;  // THIS!
}
```

**Cause 2: Counter not initialized**
```c
// âŒ Wrong
void player_init(Player* player, int col, int row) {
    player->col = col;
    player->row = row;
    // Forgot dirt_dug!
}

// âœ… Right
void player_init(Player* player, int col, int row) {
    player->col = col;
    player->row = row;
    player->dirt_dug = 0;  // Initialize
}
```

**Debug it:**
```c
// Add print when digging
if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    player->dirt_dug++;
    printf("Dug dirt! Total: %d\n", player->dirt_dug);
}
```

### HUD Bars Not Visible (Step 5)

**Symptom:**
Dirt counter works but bars don't show on screen.

**Cause 1: HUD not being drawn**
```c
// âŒ Wrong - forgot to call render_draw_hud
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
render_draw_player(renderer, &player);
// No HUD!
SDL_RenderPresent(renderer);

// âœ… Right
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
render_draw_player(renderer, &player);
render_draw_hud(renderer, &player);  // THIS!
SDL_RenderPresent(renderer);
```

**Cause 2: HUD drawn before clear**
```c
// âŒ Wrong - order matters!
render_draw_hud(renderer, &player);  // Drawn first
SDL_RenderClear(renderer);           // Clears it!
render_draw_grid(renderer, grid);
render_draw_player(renderer, &player);

// âœ… Right - HUD last
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
render_draw_player(renderer, &player);
render_draw_hud(renderer, &player);
```

**Cause 3: Wrong color or position**
```c
// Debug HUD rendering
void render_draw_hud(SDL_Renderer *renderer, Player *player) {
    int bars = player->dirt_dug / 10;
    printf("Drawing %d bars for %d dirt\n", bars, player->dirt_dug);
    
    // Make bars bright and obvious for testing
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Bright red
    
    SDL_FRect bar;
    bar.w = 20;
    bar.h = 10;
    bar.y = 5;
    
    for (int i = 0; i < bars && i < 30; i++) {
        bar.x = 5 + (i * 22);
        printf("Bar %d at x=%f\n", i, bar.x);
        SDL_RenderFillRect(renderer, &bar);
    }
}
```

### Player "Stutters" When Moving (Step 5)

**Symptom:**
Movement feels jerky or inconsistent.

**Cause: Slowdown values don't match feel**
```c
// Experiment with different values
if (just_dug) {
    player->move_slowdown = 6;   // Try 4, 6, 8, 10
} else {
    player->move_slowdown = 2;   // Try 1, 2, 3, 4
}

// Or remove slowdown during testing
if (just_dug) {
    player->move_slowdown = 0;  // No delay
} else {
    player->move_slowdown = 0;
}
```

**Frame rate issues:**
```c
// Make sure frame delay is consistent
while (running) {
    // Game logic
    SDL_Delay(16);  // EXACTLY 16ms for 60 FPS
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
â¬œ **Step 4**: Player character
=======
âœ… **Step 4**: Player character
>>>>>>> commit_hash

# Keep what you want, remove markers
âœ… **Step 4**: Player character

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
// âŒ Wrong
while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        // Handle keys
    }
    // Forgot to check QUIT!
}

// âœ… Right
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
// âŒ Wrong
if (event.type == SDL_EVENT_KEY_UP) {  // Release, not press!

// âœ… Right
if (event.type == SDL_EVENT_KEY_DOWN) {
```

**Cause 2: Wrong key constant**
```c
// âŒ Wrong (SDL2 style)
if (event.key.keysym.sym == SDLK_UP) {

// âœ… Right (SDL3 style)
if (event.key.key == SDLK_UP) {
```

**Cause 3: Not polling events**
```c
// âŒ Wrong - single poll
SDL_PollEvent(&event);

// âœ… Right - loop until no more events
while (SDL_PollEvent(&event)) {
    // Handle event
}
```

**Cause 4: Slowdown preventing movement (Step 5)**
```c
// Key is pressed but slowdown blocks it
if (player->move_slowdown > 0) {
    return false;  // Can't move yet
}

// Debug it
printf("Key pressed, slowdown = %d\n", player->move_slowdown);
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
// âŒ Wrong
SDL_Quit();
// Forgot to destroy window and renderer!

// âœ… Right
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
```

**2. Creating objects in loop**
```c
// âŒ Wrong
while (running) {
    SDL_Texture* tex = SDL_CreateTexture(...);
    // Use tex
    // Forgot to destroy!
}

// âœ… Right
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
// âŒ Wrong
if (new_col < 0 || new_col > GRID_WIDTH) {  // Should be >=

// âœ… Right
if (new_col < 0 || new_col >= GRID_WIDTH) {
```

### Dirt Counter Arithmetic Wrong (Step 5)

**Issue: Integer division**
```c
// Want: 15 dirt â†’ 1 full bar + half bar
// Got:  15 dirt â†’ 1 bar (integer division!)

int bars = player->dirt_dug / 10;  // 15 / 10 = 1

// If you want partial bars, need floats
float progress = (float)player->dirt_dug / 10.0f;
int full_bars = (int)progress;
float partial = progress - full_bars;

// But for simplicity, integer division is fine
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

## Step 5 Specific Issues

### Summary Checklist

If something's wrong in Step 5, check:

1. **Struct fields added?**
   - `int dirt_dug;`
   - `int move_slowdown;`

2. **Fields initialized?**
   - `player->dirt_dug = 0;`
   - `player->move_slowdown = 0;`

3. **player_update() exists and is called?**
   ```c
   void player_update(Player* p) {
       if (p->move_slowdown > 0) p->move_slowdown--;
   }
   ```

4. **Digging increments counter?**
   ```c
   player->dirt_dug++;
   ```

5. **Slowdown applied after actions?**
   ```c
   if (just_dug) player->move_slowdown = 8;
   else player->move_slowdown = 3;
   ```

6. **Upward digging blocked?**
   ```c
   if (destination == TILE_DIRT && dir == DIR_UP) return false;
   ```

7. **HUD function called?**
   ```c
   render_draw_hud(renderer, &player);
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
   printf("DEBUG: move_slowdown=%d\n", player->move_slowdown);
   printf("DEBUG: dirt_dug=%d\n", player->dirt_dug);
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
   (gdb) break player_move
   (gdb) run
   (gdb) print player->move_slowdown
```

7. **Check the docs**
   - SDL3 wiki
   - C reference
   - Man pages

8. **Take a break**
   - Fresh eyes find bugs faster
   - Rubber duck debugging works!

## Step 6 Specific Issues

### Summary Checklist

If something's wrong in Step 6, check:

1. **Missing includes in main.c?**
   - `#include <stdlib.h>` for srand() and rand()
   - `#include <time.h>` for time()
   - `#include "enemy.h"` for enemy system

2. **Enemy files created?**
   - enemy.h exists
   - enemy.c exists
   - Added to Makefile SOURCES and OBJECTS

3. **Enemies spawned in main.c?**
   ```c
   Enemy enemies[MAX_ENEMIES];
   int enemy_count = 0;
   enemy_init(&enemies[enemy_count++], ENEMY_POOKA, 15, 5);
   ```

4. **Enemies updated each frame?**
   ```c
   for (int i = 0; i < enemy_count; i++) {
       enemy_update(&enemies[i], &player, grid);
   }
   ```

5. **Enemies rendered?**
   ```c
   render_draw_enemies(renderer, enemies, enemy_count);
   ```

6. **Render order correct?**
   - Grid first
   - Enemies second
   - Player third (on top)

### Enemies Don't Appear

**Symptom:**
Black screen or no red/green squares visible.

**Cause 1: Not spawning enemies**
```c
// âŒ Wrong - forgot to spawn
Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
// No enemy_init() calls!

// âœ… Right
Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
enemy_init(&enemies[enemy_count++], ENEMY_POOKA, 15, 5);
enemy_init(&enemies[enemy_count++], ENEMY_FYGAR, 10, 8);
```

**Cause 2: Not rendering enemies**
```c
// âŒ Wrong - forgot to call render function
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
render_draw_player(renderer, &player);
// No render_draw_enemies()!

// âœ… Right
SDL_RenderClear(renderer);
render_draw_grid(renderer, grid);
render_draw_enemies(renderer, enemies, enemy_count);  // THIS!
render_draw_player(renderer, &player);
```

**Cause 3: Enemies spawned off-screen**
```c
// Check spawn positions are valid
printf("Spawning enemy at (%d, %d)\n", col, row);
// Make sure 0 <= col < GRID_WIDTH and 0 <= row < GRID_HEIGHT
```

**Debug it:**
```c
printf("enemy_count = %d\n", enemy_count);
for (int i = 0; i < enemy_count; i++) {
    printf("Enemy %d: type=%d, pos=(%d,%d), alive=%d\n",
           i, enemies[i].type, enemies[i].col, enemies[i].row, 
           enemies[i].is_alive);
}
```

### Enemies Don't Move

**Symptom:**
Enemies spawn but stay in one place.

**Cause 1: Not updating enemies**
```c
// âŒ Wrong - forgot to update
player_update(&player);
// No enemy updates!

// âœ… Right
player_update(&player);
for (int i = 0; i < enemy_count; i++) {
    enemy_update(&enemies[i], &player, grid);
}
```

**Cause 2: Cooldown stuck**
```c
// Debug cooldown
void enemy_update(Enemy* enemy, Player* player, ...) {
    printf("Enemy cooldown: %d\n", enemy->move_cooldown);
    if (enemy->move_cooldown > 0) {
        enemy->move_cooldown--;
        return;
    }
    // ... movement logic ...
}
```

**Cause 3: AI not working**
```c
// Debug AI decisions
Direction preferred = get_direction_to(...);
printf("Enemy at (%d,%d), Player at (%d,%d), preferred dir=%d\n",
       enemy->col, enemy->row, player->col, player->row, preferred);
```

**Cause 4: All moves blocked**
```c
// Debug movement attempts
bool moved = enemy_try_move(enemy, preferred, grid);
printf("Tried to move %d, success=%d\n", preferred, moved);
```

### Enemies Move Too Fast

**Symptom:**
Enemies zoom around and catch player instantly.

**Solution: Increase cooldown values in enemy.c**
```c
// In enemy_try_move():
if (tile == TILE_DIRT) {
    enemy->move_cooldown = 20;  // Increase from 12
} else {
    enemy->move_cooldown = 10;  // Increase from 6
}
```

**Test different values:**
```c
// Very slow (easier)
enemy->move_cooldown = 30;

// Slow (easy)
enemy->move_cooldown = 20;

// Medium (balanced)
enemy->move_cooldown = 10;

// Fast (hard)
enemy->move_cooldown = 5;

// Very fast (very hard)
enemy->move_cooldown = 3;
```

### Enemies Too Good at Chasing

**Symptom:**
Enemies perfectly track player, game too hard.

**Solution: Add randomness in enemy.c**
```c
void enemy_update(Enemy* enemy, Player* player, ...) {
    // ... cooldown check ...
    
    // Add random movement
    if (rand() % 100 < 30) {  // 30% chance
        Direction random_dir = rand() % 4;
        if (enemy_try_move(enemy, random_dir, grid)) {
            return;
        }
    }
    
    // Normal chase logic
    Direction preferred = get_direction_to(...);
    // ...
}
```

**Tune randomness:**
```c
// More random = dumber enemies
if (rand() % 100 < 50) { ... }  // 50% random

// Less random = smarter enemies
if (rand() % 100 < 10) { ... }  // 10% random
```

### Implicit Declaration of srand/rand

**Symptom:**
```
main.c:42:3: error: implicit declaration of function 'srand'
```

**Solution:**
Add to top of main.c:
```c
#include <stdlib.h>  // For srand() and rand()
#include <time.h>    // For time()
```

### Enemies Walk Through Walls

**Debug the pathfinding:**
```c
static bool enemy_try_move(Enemy* enemy, Direction dir, ...) {
    // ... calculate new_col, new_row ...
    
    TileType tile = grid_get_tile(grid, new_row, new_col);
    printf("Enemy trying to move to (%d,%d), tile=%d\n", 
           new_col, new_row, tile);
    
    if (tile == TILE_ROCK) {
        printf("  Blocked by rock!\n");
        return false;
    }
    // ...
}
```

**Check enemy_can_walk() logic:**
```c
static bool enemy_can_walk(TileType tile) {
    printf("Can walk on tile %d? %d\n", tile, 
           tile == TILE_EMPTY || tile == TILE_TUNNEL || tile == TILE_DIRT);
    return (tile == TILE_EMPTY || tile == TILE_TUNNEL || tile == TILE_DIRT);
}
```

### Collision Not Working

**Symptom:**
Player and enemy overlap, no "Game over!" message.

**Cause 1: Not checking collision**
```c
// âŒ Wrong - forgot to check
for (int i = 0; i < enemy_count; i++) {
    enemy_update(&enemies[i], &player, grid);
    // No collision check!
}

// âœ… Right
for (int i = 0; i < enemy_count; i++) {
    enemy_update(&enemies[i], &player, grid);
    if (enemy_collides_with_player(&enemies[i], &player)) {
        printf("Hit by enemy! Game over!\n");
        player.is_alive = false;
    }
}
```

**Cause 2: Wrong collision logic**
```c
// Debug collision
bool enemy_collides_with_player(Enemy* enemy, Player* player) {
    bool collides = (enemy->is_alive && 
                     player->is_alive &&
                     enemy->col == player->col &&
                     enemy->row == player->row);
    printf("Collision check: E(%d,%d) P(%d,%d) = %d\n",
           enemy->col, enemy->row, player->col, player->row, collides);
    return collides;
}
```

### Forward Declaration Errors

**Symptom:**
```
render.h:10:45: error: unknown type name 'Enemy'
```

**Solution:**
Add forward declaration in render.h:
```c
// At top of render.h
typedef struct Player Player;
typedef struct Enemy Enemy;

// Then use pointers
void render_draw_player(SDL_Renderer* renderer, Player* player);
void render_draw_enemies(SDL_Renderer* renderer, Enemy enemies[], int count);
```

### Enemies Render Behind Grid

**Symptom:**
Can barely see enemies, they appear behind dirt.

**Cause: Wrong render order**
```c
// âŒ Wrong order
render_draw_enemies(renderer, enemies, enemy_count);  // First
render_draw_grid(renderer, grid);  // Grid covers enemies!

// âœ… Right order
render_draw_grid(renderer, grid);  // Background
render_draw_enemies(renderer, enemies, enemy_count);  // On top
render_draw_player(renderer, &player);  // Player on top of enemies
```

### Compiler Warnings About Enum Comparison

**Symptom:**
```
enemy.c:120:11: warning: comparison of integer expressions 
of different signedness: 'int' and 'Direction'
```

**Solution:**
Change loop variable type:
```c
// âŒ Wrong
for (int d = 0; d < 4; d++) {
    if (d != preferred) { ... }
}

// âœ… Right
for (Direction d = DIR_UP; d <= DIR_RIGHT; d++) {
    if (d != preferred) { ... }
}
```

### Enemies Get Stuck in Corners

**This is actually OK** - the simple AI can get stuck sometimes. To fix:

**Add more alternatives:**
```c
// Try all 4 directions instead of just alternatives
Direction all_dirs[4] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
for (int i = 0; i < 4; i++) {
    if (enemy_try_move(enemy, all_dirs[i], grid)) {
        return;  // Found a valid move
    }
}
// Still stuck? That's OK, try again next frame
```

**Or add backtracking:**
```c
// Remember last direction
enemy->last_dir = dir;

// If stuck, try opposite direction
Direction opposite = get_opposite(enemy->last_dir);
enemy_try_move(enemy, opposite, grid);
```

### Random Behavior Not Random

**Symptom:**
Enemies always move the same way each run.

**Cause: Forgot to seed random**
```c
// âŒ Wrong - no seed
int main(void) {
    // ... SDL init ...
    // Forgot srand()!
    
// âœ… Right
int main(void) {
    srand(time(NULL));  // Seed at startup
    // ... rest of init ...
}
```

**Check it's seeded:**
```c
printf("First random: %d\n", rand());  // Should be different each run
```


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
   - "I added printf, slowdown shows 0"

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

- Read error messages carefully  
- Use printf debugging liberally  
- Check assumptions with prints  
- Enable compiler warnings  
- Test small parts in isolation  
- Use version control (git)  
- Initialize all struct fields  
- Check function call order  
- Verify cooldown systems work  
- Debug HUD rendering separately  
- Take breaks when stuck  
- Search before asking  
- Provide context when asking for help
