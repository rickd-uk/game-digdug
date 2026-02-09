# Key Programming Concepts

## C Programming Fundamentals

### 1. Pointers

**What is a pointer?**
A variable that stores a memory address.
```c
int x = 42;      // Regular variable
int* ptr = &x;   // Pointer to x (stores address of x)

printf("%d\n", x);      // Prints: 42
printf("%d\n", *ptr);   // Prints: 42 (dereference pointer)
printf("%p\n", ptr);    // Prints: 0x7fff5fbff8ac (address)
```

**The `&` operator** (address-of):
```c
int value = 100;
int* ptr = &value;  // Get address of value
```

**The `*` operator** (dereference):
```c
int* ptr = &value;
*ptr = 200;  // Change value through pointer
// value is now 200
```

**Why pointers?**
```c
// Without pointers - function can't modify original
void increment(int x) {
    x++;  // Modifies copy
}

int value = 5;
increment(value);
// value is still 5

// With pointers - function modifies original
void increment(int* x) {
    (*x)++;  // Modifies original
}

int value = 5;
increment(&value);
// value is now 6
```

### 2. Structs

**Defining a struct:**
```c
typedef struct {
    int x;
    int y;
    char name[20];
} Point;
```

**Using structs:**
```c
Point p;           // Declare
p.x = 10;          // Access with .
p.y = 20;

Point* ptr = &p;   // Pointer to struct
ptr->x = 30;       // Access with ->
ptr->y = 40;
```

**Struct with functions:**
```c
void move_point(Point* p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

Point p = {0, 0};
move_point(&p, 5, 10);  // p is now (5, 10)
```

**Why typedef?**
```c
// Without typedef
struct Point {
    int x, y;
};
struct Point p;  // Need "struct" keyword

// With typedef
typedef struct {
    int x, y;
} Point;
Point p;  // Cleaner!
```

### 3. Enums

**Basic enum:**
```c
typedef enum {
    RED,      // 0
    GREEN,    // 1
    BLUE      // 2
} Color;
```

**Explicit values:**
```c
typedef enum {
    ERROR = -1,
    SUCCESS = 0,
    WARNING = 1
} Status;
```

**Using enums:**
```c
Color favorite = RED;

switch(favorite) {
    case RED:
        printf("Red!\n");
        break;
    case GREEN:
        printf("Green!\n");
        break;
    case BLUE:
        printf("Blue!\n");
        break;
}
```

**Why enums?**
- Better than magic numbers
- Compiler checks valid values
- Self-documenting code
- IDE autocomplete

### 4. Arrays

**1D Arrays:**
```c
int scores[5];          // Declare
scores[0] = 100;        // Access
scores[4] = 95;

// Initialize
int scores[5] = {100, 95, 87, 92, 88};
```

**2D Arrays:**
```c
int grid[3][4];         // 3 rows, 4 columns

// Always [row][col], not [col][row]!
grid[0][0] = 1;         // Top-left
grid[2][3] = 9;         // Bottom-right

// Initialize
int grid[3][4] = {
    {1, 2, 3, 4},       // Row 0
    {5, 6, 7, 8},       // Row 1
    {9, 10, 11, 12}     // Row 2
};
```

**Array loops:**
```c
// 1D
for (int i = 0; i < 5; i++) {
    printf("%d ", scores[i]);
}

// 2D
for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 4; col++) {
        printf("%d ", grid[row][col]);
    }
    printf("\n");
}
```

**Arrays and pointers:**
```c
int arr[5] = {1, 2, 3, 4, 5};
int* ptr = arr;  // Array name is pointer to first element

printf("%d\n", arr[2]);    // 3
printf("%d\n", *(ptr + 2)); // 3 (same thing)
```

### 5. Header Files

**What are header files?**
- .h files contain declarations
- .c files contain implementations
- #include "header.h" brings in declarations

**Example header (math.h):**
```c
#ifndef MATH_H
#define MATH_H

int add(int a, int b);
int multiply(int a, int b);

#endif
```

**Implementation (math.c):**
```c
#include "math.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}
```

**Using it (main.c):**
```c
#include "math.h"

int main() {
    int result = add(5, 3);
    return 0;
}
```

### 6. Header Guards

**Problem without guards:**
```c
// math.h
typedef struct {
    int x, y;
} Point;

// main.c
#include "math.h"
#include "geometry.h"  // Also includes math.h
// ERROR: Point defined twice!
```

**Solution - header guards:**
```c
#ifndef MATH_H     // If not defined
#define MATH_H     // Define it

typedef struct {
    int x, y;
} Point;

#endif             // End if
```

**How it works:**
```
First include:
  MATH_H not defined â†’ enter block â†’ define MATH_H â†’ define Point

Second include:
  MATH_H already defined â†’ skip block â†’ Point not redefined
```

**Alternative - #pragma once:**
```c
#pragma once

typedef struct {
    int x, y;
} Point;
```

Less portable but simpler.

### 7. Static Keyword

**Static in functions:**
```c
// player.c
static bool can_move_to(TileType tile) {
    // Only visible in player.c
    return tile == TILE_TUNNEL;
}
```

**Why use static?**
- Helper function, not part of public API
- Prevents naming conflicts
- Clearer code organization

**Static in variables:**
```c
void counter() {
    static int count = 0;  // Initialized once
    count++;
    printf("%d\n", count);
}

counter();  // Prints: 1
counter();  // Prints: 2
counter();  // Prints: 3
```

**Static vs non-static:**
```c
void func1() {
    int x = 0;  // Reset every call
    x++;
}

void func2() {
    static int x = 0;  // Persists between calls
    x++;
}
```

### 8. Const Keyword

**Const variables:**
```c
const int MAX_PLAYERS = 4;
MAX_PLAYERS = 5;  // ERROR: Can't modify
```

**Const pointers:**
```c
const int* ptr;        // Can't modify what it points to
int* const ptr;        // Can't change pointer itself
const int* const ptr;  // Can't modify either
```

**Const in functions:**
```c
void print_player(const Player* player) {
    // Can read player, can't modify it
    printf("%d, %d\n", player->col, player->row);
    player->col = 5;  // ERROR: Can't modify
}
```

### 9. Boolean Flags

**Using bool for state:**
```c
bool just_dug = false;
if (destination == TILE_DIRT) {
    // Do digging work
    just_dug = true;
}

if (just_dug) {
    // Different behavior based on what happened
    apply_slowdown();
}
```

**Why use flags?**
- Store temporary state within a function
- Makes conditional logic clearer
- Avoids repeating complex checks

**Common pattern - action detection:**
```c
bool player_move(...) {
    bool moved = false;
    bool dug = false;
    
    // Try to move
    if (can_move) {
        // Do move
        moved = true;
    }
    
    // Try to dig
    if (is_dirt) {
        // Do dig
        dug = true;
    }
    
    // Apply consequences based on what happened
    if (dug) apply_dig_cooldown();
    if (moved) play_move_sound();
    
    return moved || dug;
}
```

## Game Programming Patterns

### 1. Game Loop
```
Initialize
   â†“
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚  Handle Input   â”‚
â”‚       â†“         â”‚
â”‚  Update State   â”‚ â† Repeat forever
â”‚       â†“         â”‚
â”‚     Render      â”‚
â”‚       â†“         â”‚
â”‚  Frame Delay    â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
   â†“
Cleanup
```

**Code structure:**
```c
init_game();

while (running) {
    handle_input();
    update_game();
    render();
    delay();
}

cleanup();
```

### 2. Frame-Based Timing

**The problem:**
```c
// Button held down
while (button_pressed) {
    player_move();  // Moves EVERY frame!
}
// Player teleports across screen
```

**Solution - cooldown/delay system:**
```c
typedef struct {
    int move_slowdown;  // Frames until can move again
} Player;

// When action happens
void player_move(...) {
    // Do the move
    player->move_slowdown = 8;  // Wait 8 frames
}

// Every frame
void player_update(Player* p) {
    if (p->move_slowdown > 0) {
        p->move_slowdown--;  // Count down
    }
}

// When trying to act
bool can_act(Player* p) {
    return p->move_slowdown == 0;
}
```

**Real-world timing examples:**
```c
// Fast action (3 frames @ 60 FPS = 50ms)
player->attack_cooldown = 3;

// Slow action (30 frames @ 60 FPS = 500ms)
player->spell_cooldown = 30;

// Very slow (120 frames @ 60 FPS = 2 seconds)
player->ultimate_cooldown = 120;
```

**Multiple timers pattern:**
```c
typedef struct {
    int move_cooldown;
    int attack_cooldown;
    int invulnerable_timer;
    int animation_timer;
} Player;

void player_update(Player* p) {
    // Update all timers every frame
    if (p->move_cooldown > 0) p->move_cooldown--;
    if (p->attack_cooldown > 0) p->attack_cooldown--;
    if (p->invulnerable_timer > 0) p->invulnerable_timer--;
    if (p->animation_timer > 0) p->animation_timer--;
}
```

### 3. Context-Sensitive Actions

**Different behavior based on situation:**
```c
bool player_move(...) {
    TileType dest = get_tile(new_pos);
    
    // Same action (move), different consequences
    if (dest == TILE_DIRT) {
        dig_tile(new_pos);
        move_slowdown = 8;      // Slow
        play_dig_sound();
    } else if (dest == TILE_TUNNEL) {
        move_slowdown = 3;      // Fast
        play_step_sound();
    }
}
```

**Why this matters:**
- Same input â†’ different feel based on context
- Creates depth and strategy
- Makes world feel interactive

**Examples from games:**
```c
// Walking vs swimming
if (in_water) {
    speed = 0.5;  // Slow
} else {
    speed = 1.0;  // Normal
}

// Sneaking vs running
if (crouched) {
    speed = 0.3;   // Very slow
    noise = 0;     // Silent
} else {
    speed = 1.0;   // Fast
    noise = 1.0;   // Loud
}
```

### 4. Game Feel Through Timing

**What is "game feel"?**
The tactile sensation of playing - how responsive and satisfying actions feel.

**Bad feel - instant:**
```c
void player_jump() {
    player->y = target_height;  // Teleports instantly
}
// Feels: Floaty, unsatisfying, cheap
```

**Good feel - weighted:**
```c
void player_jump() {
    player->jump_velocity = -15;  // Initial force
    player->jump_cooldown = 30;   // Can't jump again immediately
}

void player_update() {
    // Gravity and physics over time
    player->y += player->jump_velocity;
    player->jump_velocity += GRAVITY;
}
// Feels: Weighty, satisfying, real
```

**Dig Dug timing example:**
```c
// Digging - heavy, deliberate
if (just_dug) {
    move_slowdown = 8;  // 133ms delay
}

// Running through tunnels - responsive
else {
    move_slowdown = 3;  // 50ms delay
}
```

**Key principles:**
- **Weight** - Actions take time
- **Feedback** - Visual/audio confirmation
- **Distinction** - Different actions feel different
- **Consistency** - Similar actions feel similar

### 5. State Tracking

**What to track:**
```c
typedef struct {
    // Position
    int x, y;
    
    // Statistics
    int score;
    int lives;
    int ammo;
    
    // Progress
    int enemies_killed;
    int dirt_dug;
    int levels_completed;
    
    // State flags
    bool is_invulnerable;
    bool has_key;
    bool is_powered_up;
} Player;
```

**Why track state?**
- Score and achievements
- Unlock systems
- Difficulty scaling
- Player feedback

**Using tracked state:**
```c
// Score multiplier based on depth
int get_score(int depth, int dirt_dug) {
    int multiplier = 1 + (depth / 5);
    return dirt_dug * 10 * multiplier;
}

// Unlock achievement
if (player->dirt_dug >= 1000) {
    unlock_achievement("Mole Master");
}

// Scale difficulty
int enemy_count = 3 + (player->level / 2);
```

### 6. Entity-Component Pattern

**Entity:** Game object
**Component:** Data that describes the entity
```c
// Simple entity (struct groups components)
typedef struct {
    Position pos;      // Position component
    Velocity vel;      // Velocity component
    Sprite sprite;     // Sprite component
    Health health;     // Health component
} Entity;
```

**Functions operate on components:**
```c
void update_position(Position* pos, Velocity* vel) {
    pos->x += vel->dx;
    pos->y += vel->dy;
}

void render_sprite(Sprite* sprite, Position* pos) {
    draw_sprite(sprite->image, pos->x, pos->y);
}
```

### 7. Separation of Concerns

**Each module has ONE job:**
```
Input Module: "What did the user do?"
  â†“
Logic Module: "What should change?"
  â†“
Render Module: "How do we show it?"
```

**Bad - mixed concerns:**
```c
void player_update() {
    // Handle input
    if (key_pressed(UP)) { ... }
    
    // Update state
    player.y--;
    
    // Render
    draw_sprite(player.sprite, player.x, player.y);
}
```

**Good - separated:**
```c
void handle_input(Player* player) {
    if (key_pressed(UP)) {
        player->move_up = true;
    }
}

void update_player(Player* player) {
    if (player->move_up) {
        player->y--;
    }
}

void render_player(Player* player) {
    draw_sprite(player->sprite, player->x, player->y);
}
```

### 8. Rule-Based Mechanics

**Adding constraints creates gameplay:**
```c
bool can_move_to(int x, int y, Direction dir) {
    TileType tile = get_tile(x, y);
    
    // Rule 1: Can't walk through walls
    if (tile == TILE_WALL) return false;
    
    // Rule 2: Can't dig upward
    if (tile == TILE_DIRT && dir == DIR_UP) return false;
    
    // Rule 3: Need key for doors
    if (tile == TILE_DOOR && !player.has_key) return false;
    
    return true;
}
```

**Why rules matter:**
- Create puzzles and challenges
- Force strategic thinking
- Make world feel consistent
- Limit player power

**Examples from games:**
```c
// Zelda: Need specific item for obstacle
if (obstacle == BOMB_WALL && !has_bombs) return false;

// Mario: Can't break brick without power-up
if (block == BRICK && player.size == SMALL) return false;

// Dig Dug: Can't dig upward (this project!)
if (tile == TILE_DIRT && dir == DIR_UP) return false;
```

### 9. Data-Driven Design

**Code-driven (hard-coded):**
```c
if (level == 1) {
    spawn_enemy(100, 100, POOKA);
    spawn_enemy(200, 150, FYGAR);
} else if (level == 2) {
    spawn_enemy(50, 80, POOKA);
    spawn_enemy(150, 120, POOKA);
    spawn_enemy(250, 160, FYGAR);
}
```

**Data-driven (from file):**
```c
// level1.txt:
// enemy pooka 100 100
// enemy fygar 200 150

load_level_from_file("level1.txt");
```

**Benefits:**
- Easy to modify levels
- No recompilation needed
- Level designers can work independently
- Easy to add content

### 10. State Machines

**Game states:**
```c
typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER
} GameState;

GameState current_state = STATE_MENU;

void update() {
    switch(current_state) {
        case STATE_MENU:
            update_menu();
            break;
        case STATE_PLAYING:
            update_game();
            break;
        case STATE_PAUSED:
            update_pause_menu();
            break;
        case STATE_GAME_OVER:
            update_game_over();
            break;
    }
}
```

**State transitions:**
```
MENU â†’ PLAYING (start game)
PLAYING â†’ PAUSED (press pause)
PAUSED â†’ PLAYING (unpause)
PLAYING â†’ GAME_OVER (player dies)
GAME_OVER â†’ MENU (restart)
```

## Compilation and Linking

### Compilation Process
```
Source Files â†’ Preprocessor â†’ Compiler â†’ Assembler â†’ Linker â†’ Executable

main.c    â”
grid.c    â”œâ†’ [Preprocess] â†’ [Compile] â†’ [Assemble] â†’ main.o
render.c  â”˜                                            grid.o
                                                       render.o
                                                          â†“
                                                      [Link]
                                                          â†“
                                                       digdug
```

**Preprocessor:**
- Handles #include, #define, #ifdef
- Text substitution
- Output: expanded source code

**Compiler:**
- Translates C to assembly
- Optimizes code
- Output: assembly code

**Assembler:**
- Translates assembly to machine code
- Output: object files (.o)

**Linker:**
- Combines object files
- Resolves function calls
- Links libraries
- Output: executable

### Manual Compilation
```bash
# Preprocess only (-E)
gcc -E main.c -o main.i

# Compile to assembly (-S)
gcc -S main.c -o main.s

# Assemble to object (-c)
gcc -c main.c -o main.o

# Link multiple objects
gcc main.o grid.o render.o -lSDL3 -o digdug

# All in one
gcc main.c grid.c render.c -lSDL3 -o digdug
```

### Libraries

**Static libraries (.a):**
- Compiled into executable
- Larger executable
- No runtime dependencies

**Shared/dynamic libraries (.so, .dll, .dylib):**
- Loaded at runtime
- Smaller executable
- Requires library file at runtime

**Linking libraries:**
```bash
gcc main.o -lSDL3 -o digdug
#           ^^^^ library name (lib prefix automatic)
# Linker searches for libSDL3.so or libSDL3.a
```

**pkg-config:**
```bash
# Get compiler flags
pkg-config --cflags sdl3
# Output: -I/usr/include/SDL3

# Get linker flags
pkg-config --libs sdl3
# Output: -lSDL3

# Use in compilation
gcc $(pkg-config --cflags sdl3) main.c $(pkg-config --libs sdl3)
```

## Memory Management

### Stack vs Heap

**Stack:**
```c
void func() {
    int x = 5;        // Stack
    char buf[100];    // Stack
}  // x and buf automatically destroyed
```

**Heap:**
```c
void func() {
    int* ptr = malloc(sizeof(int) * 100);  // Heap
    // Use ptr...
    free(ptr);  // Must manually free!
}
```

**When to use each:**
- Stack: Small, short-lived, fixed size
- Heap: Large, long-lived, dynamic size

### Common Memory Errors

**Buffer overflow:**
```c
char buf[10];
strcpy(buf, "This is too long!");  // Overflow!
```

**Use after free:**
```c
int* ptr = malloc(100);
free(ptr);
*ptr = 5;  // ERROR: Use after free
```

**Memory leak:**
```c
void func() {
    int* ptr = malloc(100);
    // Forgot to free!
}  // ptr lost, memory leaked
```

**Double free:**
```c
int* ptr = malloc(100);
free(ptr);
free(ptr);  // ERROR: Double free
```

## Debugging Tips

### Printf Debugging
```c
printf("DEBUG: x = %d, y = %d\n", x, y);
printf("DEBUG: Entering function\n");
printf("DEBUG: condition = %s\n", condition ? "true" : "false");

// Debug timing
printf("DEBUG: slowdown = %d\n", player->move_slowdown);
printf("DEBUG: just_dug = %s\n", just_dug ? "true" : "false");
```

### Assert
```c
#include <assert.h>

assert(player != NULL);
assert(x >= 0 && x < GRID_WIDTH);
assert(score >= 0);
assert(move_slowdown >= 0);  // Never negative
```

Crashes if condition is false - catches bugs early!

### GDB Basics
```bash
# Compile with debug symbols
gcc -g main.c -o main

# Run in GDB
gdb ./main

# GDB commands
(gdb) break main        # Set breakpoint
(gdb) run               # Start program
(gdb) next              # Next line
(gdb) step              # Step into function
(gdb) print x           # Print variable
(gdb) print player->move_slowdown  # Print struct field
(gdb) continue          # Continue execution
(gdb) quit              # Exit GDB
```


### 11. Entity Arrays

**Managing multiple game objects:**
```c
#define MAX_ENEMIES 10

Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
```

**Why arrays for entities?**
- Fixed-size, stack-allocated
- Easy to iterate over all entities
- No memory management needed (for now)
- Cache-friendly (data in contiguous memory)

**Adding entities:**
```c
void spawn_enemy(int col, int row, EnemyType type) {
    if (enemy_count < MAX_ENEMIES) {
        enemy_init(&enemies[enemy_count], type, col, row);
        enemy_count++;
    }
}
```

**Updating all entities:**
```c
for (int i = 0; i < enemy_count; i++) {
    enemy_update(&enemies[i], &player, grid);
}
```

**Removing dead entities:**
```c
// Swap-and-pop removal (fast, changes order)
for (int i = 0; i < enemy_count; i++) {
    if (!enemies[i].is_alive) {
        enemies[i] = enemies[enemy_count - 1];  // Swap with last
        enemy_count--;                          // Decrease count
        i--;                                    // Recheck this index
    }
}
```

**Why swap-and-pop?**
- O(1) removal (constant time)
- No need to shift all elements
- Order doesn't matter for most games
- Very cache-friendly

**Alternative - maintain order:**
```c
// Slower but preserves order
for (int i = 0; i < enemy_count; i++) {
    if (!enemies[i].is_alive) {
        // Shift all subsequent elements left
        for (int j = i; j < enemy_count - 1; j++) {
            enemies[j] = enemies[j + 1];
        }
        enemy_count--;
        i--;
    }
}
```

### 12. Random Number Generation

**Basic random numbers:**
```c
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));  // Seed once at startup
    
    int random_num = rand();  // 0 to RAND_MAX
}
```

**Common patterns:**

**Random in range [0, n):**
```c
int random_index = rand() % 10;  // 0 to 9
int random_col = rand() % GRID_WIDTH;
```

**Random in range [min, max]:**
```c
int random_value = min + rand() % (max - min + 1);
int random_row = 5 + rand() % 10;  // 5 to 14
```

**Random boolean (50/50 chance):**
```c
bool coin_flip = rand() % 2;  // 0 or 1
if (rand() % 2) {
    // 50% chance
}
```

**Random percentage chance:**
```c
if (rand() % 100 < 30) {
    // 30% chance
}

if (rand() % 100 < 75) {
    // 75% chance
}
```

**Random direction:**
```c
Direction random_dir = (Direction)(rand() % 4);
// DIR_UP=0, DIR_DOWN=1, DIR_LEFT=2, DIR_RIGHT=3
```

**Random enum:**
```c
EnemyType random_type = (rand() % 2 == 0) ? ENEMY_POOKA : ENEMY_FYGAR;
```

**Why seed with time()?**
```c
srand(time(NULL));  // Different seed each run
```
- time(NULL) returns current time in seconds
- Different seed = different random sequence
- Without srand(): same sequence every run (deterministic)

**When NOT to seed:**
- Testing/debugging (want reproducible behavior)
- Replays (want same sequence)
- Network games (need synchronized random)

**Better random (if needed):**
```c
// For better randomness, use rand() result as index
int better_random() {
    return (rand() ^ (rand() << 15)) % max_value;
}
```

### 13. Forward Declarations

**The problem:**
```c
// render.h
#include "player.h"
#include "enemy.h"

// player.h
#include "render.h"  // CIRCULAR!
```

**Solution - forward declare:**
```c
// render.h
typedef struct Player Player;  // Forward declaration
typedef struct Enemy Enemy;    // Forward declaration

void render_draw_player(SDL_Renderer* renderer, Player* player);
void render_draw_enemies(SDL_Renderer* renderer, Enemy enemies[], int count);
```

**When to use:**
- Breaking circular dependencies
- Header only needs pointer to type
- Don't need to know struct internals

**When NOT to use:**
```c
// âŒ Can't do this with forward declaration
typedef struct Player Player;
void use_player(Player p);  // ERROR: Need full definition

// âœ… This works
typedef struct Player Player;
void use_player(Player* p);  // OK: Just a pointer
```

**Full example:**
```c
// enemy.h
#ifndef ENEMY_H
#define ENEMY_H

// Forward declare Player (defined in player.h)
typedef struct Player Player;

typedef struct {
    int col, row;
    // ...
} Enemy;

// Uses Player* (pointer), so forward declaration is enough
void enemy_update(Enemy* enemy, Player* player, ...);

#endif
```

**Why this works:**
- Compiler knows pointers are all the same size
- Don't need struct contents to pass pointer
- Only enemy.c needs full Player definition

**In enemy.c:**
```c
#include "enemy.h"
#include "player.h"  // NOW we get full definition

void enemy_update(Enemy* enemy, Player* player, ...) {
    // Can access player->col, player->row, etc.
}
```


### 11. AI Pathfinding Basics

**Simple chase algorithm:**
```c
Direction get_direction_to(int from_col, int from_row, 
                           int to_col, int to_row) {
    int dx = to_col - from_col;
    int dy = to_row - from_row;
    
    // Move in direction of larger difference
    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? DIR_RIGHT : DIR_LEFT;
    } else {
        return (dy > 0) ? DIR_DOWN : DIR_UP;
    }
}
```

**How it works:**
```
Player at (10, 5)
Enemy at (6, 8)

dx = 10 - 6 = 4   (player is 4 tiles right)
dy = 5 - 8 = -3   (player is 3 tiles up)

abs(4) > abs(3)   (horizontal difference larger)
â†' Move RIGHT to close the gap
```

**Why this is good enough:**
- Simple to implement
- No complex data structures
- Fast (just arithmetic)
- Feels intelligent to players
- Enemies don't get stuck easily

**Making AI less perfect:**
```c
// 30% chance to move randomly instead
if (rand() % 100 < 30) {
    Direction random_dir = rand() % 4;
    return random_dir;
}

// Otherwise chase player
return get_direction_to(enemy_col, enemy_row, player_col, player_row);
```

**Why randomness helps:**
- More organic movement
- Less frustrating for player
- Enemies don't always take optimal path
- Creates variety in behavior

**Alternative AI strategies:**

**Line of sight:**
```c
bool can_see_player(Enemy* enemy, Player* player) {
    // Only chase if can see player
    if (enemy->row != player->row && enemy->col != player->col) {
        return false;  // Not in same row or column
    }
    // Check for walls between
    return true;
}
```

**State-based AI:**
```c
typedef enum {
    AI_STATE_PATROL,
    AI_STATE_CHASE,
    AI_STATE_FLEE,
    AI_STATE_ATTACK
} AIState;

void enemy_update(Enemy* enemy, Player* player) {
    switch(enemy->ai_state) {
        case AI_STATE_PATROL:
            patrol_random();
            if (can_see_player()) {
                enemy->ai_state = AI_STATE_CHASE;
            }
            break;
        case AI_STATE_CHASE:
            chase_player();
            if (close_enough()) {
                enemy->ai_state = AI_STATE_ATTACK;
            }
            break;
        // ...
    }
}
```

**Different personalities:**
```c
// Aggressive enemies: chase faster
if (enemy->type == ENEMY_FYGAR) {
    enemy->move_cooldown = 4;  // Fast
}

// Cautious enemies: keep distance
if (distance_to_player < 3) {
    move_away_from_player();  // Flee!
} else {
    chase_player();
}

// Random enemies: unpredictable
if (rand() % 100 < 50) {
    move_random();
} else {
    chase_player();
}
```

### 12. Multiple Entity Updates

**Update pattern:**
```c
// In main game loop
for (int i = 0; i < enemy_count; i++) {
    enemy_update(&enemies[i], &player, grid);
}
```

**Why loop through array:**
- All enemies get updated each frame
- Order doesn't matter (usually)
- Simple and clear
- Easy to add/remove entities

**Entity interactions:**
```c
// Check each enemy against player
for (int i = 0; i < enemy_count; i++) {
    if (enemy_collides_with_player(&enemies[i], &player)) {
        handle_collision(&enemies[i], &player);
    }
}

// Check enemies against each other (later)
for (int i = 0; i < enemy_count; i++) {
    for (int j = i + 1; j < enemy_count; j++) {
        if (enemies_collide(&enemies[i], &enemies[j])) {
            // Handle enemy-enemy collision
        }
    }
}
```

**Update order matters:**
```c
// âŒ Wrong order
render();
update_entities();
// Renders OLD state before updating!

// âœ… Correct order
update_entities();
render();
// Renders NEW state after updating
```

**Render order matters too:**
```c
// Back to front rendering
render_background();
render_grid();
render_enemies();    // Enemies first
render_player();     // Player on top
render_hud();        // HUD on top of everything
```

### 13. Context-Sensitive Speed

**Different speeds for different situations:**
```c
void enemy_try_move(Enemy* enemy, Direction dir, TileType grid[]) {
    // ... movement logic ...
    
    TileType tile = grid[new_row][new_col];
    
    if (tile == TILE_DIRT) {
        enemy->move_cooldown = 12;  // Slow in dirt
        enemy->is_ghosting = true;
    } else if (tile == TILE_TUNNEL) {
        enemy->move_cooldown = 6;   // Fast in tunnels
        enemy->is_ghosting = false;
    }
}
```

**Why vary speed:**
- Creates strategic depth
- Rewards player for digging
- Makes tunnels enemy territory
- Visual feedback (ghosting effect)
- Different feel for different situations

**Speed as game balance:**
```c
// Player speeds
if (digging) {
    player->move_cooldown = 8;   // Slow when digging
} else {
    player->move_cooldown = 3;   // Fast in tunnels
}

// Enemy speeds (slightly faster than player)
if (in_dirt) {
    enemy->move_cooldown = 10;   // Slower than player digging
} else {
    enemy->move_cooldown = 5;    // Faster than player running
}
```

**Tuning for feel:**
- Lower cooldown = faster = harder
- Higher cooldown = slower = easier
- Test and adjust until it feels right
- Different enemies can have different speeds


## Key Takeaways

- Pointers store memory addresses  
- Structs group related data  
- Enums replace magic numbers  
- Boolean flags track temporary state  
- Header guards prevent double-inclusion  
- Static limits scope  
- Entity arrays manage multiple game objects  
- Random numbers add variety and unpredictability  
- Forward declarations break circular dependencies  
- Game loop: Input → Update → Render  
- Frame-based timing creates game feel  
- Context-sensitive actions add depth  
- State tracking enables progression  
- Rule-based mechanics create challenge  
- Simple AI can feel surprisingly intelligent  
- Multiple entity updates need proper ordering  
- Speed variations create strategic depth  
- Separate concerns into modules  
- Compilation: source → object → executable  
- Stack for local, heap for dynamic  
- Always free what you malloc
