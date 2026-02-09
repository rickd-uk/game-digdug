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
  MATH_H not defined → enter block → define MATH_H → define Point

Second include:
  MATH_H already defined → skip block → Point not redefined
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

## Game Programming Patterns

### 1. Game Loop
```
Initialize
   ↓
┌─────────────────┐
│  Handle Input   │
│       ↓         │
│  Update State   │ ← Repeat forever
│       ↓         │
│     Render      │
│       ↓         │
│  Frame Delay    │
└─────────────────┘
   ↓
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

### 2. Entity-Component Pattern

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

### 3. Separation of Concerns

**Each module has ONE job:**
```
Input Module: "What did the user do?"
  ↓
Logic Module: "What should change?"
  ↓
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

### 4. Data-Driven Design

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

### 5. State Machines

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
MENU → PLAYING (start game)
PLAYING → PAUSED (press pause)
PAUSED → PLAYING (unpause)
PLAYING → GAME_OVER (player dies)
GAME_OVER → MENU (restart)
```

## Compilation and Linking

### Compilation Process
```
Source Files → Preprocessor → Compiler → Assembler → Linker → Executable

main.c    ┐
grid.c    ├→ [Preprocess] → [Compile] → [Assemble] → main.o
render.c  ┘                                            grid.o
                                                       render.o
                                                          ↓
                                                      [Link]
                                                          ↓
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
```

### Assert
```c
#include <assert.h>

assert(player != NULL);
assert(x >= 0 && x < GRID_WIDTH);
assert(score >= 0);
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
(gdb) continue          # Continue execution
(gdb) quit              # Exit GDB
```

## Key Takeaways

✅ Pointers store memory addresses  
✅ Structs group related data  
✅ Enums replace magic numbers  
✅ Header guards prevent double-inclusion  
✅ Static limits scope  
✅ Game loop: Input → Update → Render  
✅ Separate concerns into modules  
✅ Compilation: source → object → executable  
✅ Stack for local, heap for dynamic  
✅ Always free what you malloc
