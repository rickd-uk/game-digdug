# Step 6: Enemies and AI

## Goal
Add Pookas and Fygars with basic AI that chases the player through tunnels.

## What You'll Build
- Enemy entity system supporting multiple enemy types
- Basic AI: chase player through tunnels, ghost through dirt
- Enemy-player collision detection
- Visual distinction between enemy types

## Key Concepts

### 1. Enemy Types - More Enums!
```c
typedef enum {
    ENEMY_POOKA,   // Red balloon monster
    ENEMY_FYGAR    // Green fire-breathing dragon
} EnemyType;
```

**Design Note:** In the original Dig Dug:
- Pookas are simpler, just chase you
- Fygars can breathe fire (we'll add this in Step 7)
- Both can "ghost" through dirt but prefer tunnels

### 2. Enemy Struct
```c
typedef struct {
    int col;
    int row;
    EnemyType type;
    Direction facing;
    bool is_alive;
    int move_cooldown;      // Like player, enemies have timing
    bool is_ghosting;       // True when moving through dirt
} Enemy;
```

**Why these fields?**
- `col, row` - Position (same as player)
- `type` - Pooka vs Fygar
- `facing` - Which way they're looking
- `is_alive` - For death animations (later)
- `move_cooldown` - Prevents moving every frame
- `is_ghosting` - Visual feedback when in dirt

### 3. Enemy Array - Multiple Entities
```c
#define MAX_ENEMIES 10

Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;
```

**Why an array?**
- Need to track multiple enemies at once
- Easy to loop through for updates and rendering
- Fixed size for simplicity (dynamic allocation later)

## Step-by-Step Implementation

### 1. Create enemy.h

**enemy.h:**
```c
#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"
#include "player.h"
#include <stdbool.h>

#define MAX_ENEMIES 10

typedef enum {
    ENEMY_POOKA,  // Red balloon monster
    ENEMY_FYGAR   // Green dragon
} EnemyType;

typedef struct {
    int col;
    int row;
    EnemyType type;
    Direction facing;
    bool is_alive;
    int move_cooldown;
    bool is_ghosting;  // True when in dirt
} Enemy;

// Initialize enemy at position
void enemy_init(Enemy* enemy, EnemyType type, int col, int row);

// Update enemy AI (chase player)
void enemy_update(Enemy* enemy, Player* player, 
                  TileType grid[GRID_HEIGHT][GRID_WIDTH]);

// Get pixel position for rendering
void enemy_get_pixel_pos(Enemy* enemy, int* x, int* y);

// Check if enemy collides with player
bool enemy_collides_with_player(Enemy* enemy, Player* player);

#endif // ENEMY_H
```

### 2. Create enemy.c

**enemy.c:**
```c
#include "enemy.h"
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>

void enemy_init(Enemy* enemy, EnemyType type, int col, int row) {
    enemy->col = col;
    enemy->row = row;
    enemy->type = type;
    enemy->facing = DIR_LEFT;
    enemy->is_alive = true;
    enemy->move_cooldown = 0;
    enemy->is_ghosting = false;
}

// Helper: Check if tile is walkable for enemies
static bool enemy_can_walk(TileType tile) {
    // Enemies can walk through tunnels and empty space
    // They can also ghost through dirt!
    return (tile == TILE_EMPTY || tile == TILE_TUNNEL || tile == TILE_DIRT);
}

// Helper: Get direction to move towards target
static Direction get_direction_to(int from_col, int from_row, 
                                   int to_col, int to_row) {
    // Simple pathfinding: move horizontally first, then vertically
    int dx = to_col - from_col;
    int dy = to_row - from_row;
    
    // Prioritize horizontal movement
    if (abs(dx) > abs(dy)) {
        return (dx > 0) ? DIR_RIGHT : DIR_LEFT;
    } else {
        return (dy > 0) ? DIR_DOWN : DIR_UP;
    }
}

// Helper: Try to move enemy in a direction
static bool enemy_try_move(Enemy* enemy, Direction dir,
                           TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    int new_col = enemy->col;
    int new_row = enemy->row;
    
    switch(dir) {
        case DIR_UP:    new_row--; break;
        case DIR_DOWN:  new_row++; break;
        case DIR_LEFT:  new_col--; break;
        case DIR_RIGHT: new_col++; break;
    }
    
    // Bounds check
    if (new_col < 0 || new_col >= GRID_WIDTH ||
        new_row < 0 || new_row >= GRID_HEIGHT) {
        return false;
    }
    
    TileType tile = grid_get_tile(grid, new_row, new_col);
    
    // Can't move through rocks
    if (tile == TILE_ROCK) {
        return false;
    }
    
    // Check if walkable
    if (!enemy_can_walk(tile)) {
        return false;
    }
    
    // Move is valid
    enemy->col = new_col;
    enemy->row = new_row;
    enemy->facing = dir;
    
    // Are we ghosting through dirt?
    enemy->is_ghosting = (tile == TILE_DIRT);
    
    // Set cooldown based on terrain
    if (tile == TILE_DIRT) {
        enemy->move_cooldown = 12;  // Slow in dirt
    } else {
        enemy->move_cooldown = 6;   // Medium in tunnels
    }
    
    return true;
}

void enemy_update(Enemy* enemy, Player* player,
                  TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
    // Dead enemies don't move
    if (!enemy->is_alive) {
        return;
    }
    
    // Update cooldown
    if (enemy->move_cooldown > 0) {
        enemy->move_cooldown--;
        return;  // Can't move yet
    }
    
    // AI: Chase the player!
    Direction preferred = get_direction_to(enemy->col, enemy->row,
                                           player->col, player->row);
    
    // Try to move in preferred direction
    if (enemy_try_move(enemy, preferred, grid)) {
        return;  // Success!
    }
    
    // Blocked! Try alternatives
    Direction alternatives[3];
    int alt_count = 0;
    
    // Generate alternative directions (perpendicular + opposite)
    for (int d = 0; d < 4; d++) {
        if (d != preferred) {
            alternatives[alt_count++] = (Direction)d;
        }
    }
    
    // Try random alternative
    if (alt_count > 0) {
        int choice = rand() % alt_count;
        enemy_try_move(enemy, alternatives[choice], grid);
    }
}

void enemy_get_pixel_pos(Enemy* enemy, int* x, int* y) {
    *x = enemy->col * TILE_SIZE;
    *y = enemy->row * TILE_SIZE;
}

bool enemy_collides_with_player(Enemy* enemy, Player* player) {
    return (enemy->is_alive && 
            player->is_alive &&
            enemy->col == player->col &&
            enemy->row == player->row);
}
```

**AI Breakdown:**

```
enemy_update():
  1. Check cooldown (can we move?)
  2. Calculate best direction to player
  3. Try to move that way
  4. If blocked, try random alternative
  
get_direction_to():
  - Compares distances (dx, dy)
  - Picks horizontal or vertical based on which is larger
  - Simple but effective!
  
enemy_try_move():
  - Check bounds
  - Check tile type
  - Update position if valid
  - Set is_ghosting flag
  - Apply movement cooldown
```

### 3. Add Enemy Rendering to render.h/render.c

**Add to render.h:**
```c
// Forward declare Enemy (defined in enemy.h)
typedef struct Enemy Enemy;

void render_draw_enemies(SDL_Renderer* renderer, Enemy enemies[], 
                         int enemy_count);
```

**Add to render.c:**
```c
#include "enemy.h"  // Add at top

void render_draw_enemies(SDL_Renderer* renderer, Enemy enemies[], 
                         int enemy_count) {
    for (int i = 0; i < enemy_count; i++) {
        Enemy* enemy = &enemies[i];
        
        if (!enemy->is_alive) {
            continue;  // Skip dead enemies
        }
        
        SDL_FRect enemy_rect;
        int x, y;
        enemy_get_pixel_pos(enemy, &x, &y);
        
        enemy_rect.x = x;
        enemy_rect.y = y;
        enemy_rect.w = TILE_SIZE;
        enemy_rect.h = TILE_SIZE;
        
        // Different colors for different types
        if (enemy->type == ENEMY_POOKA) {
            // Pookas are red
            if (enemy->is_ghosting) {
                SDL_SetRenderDrawColor(renderer, 100, 0, 0, 180);  // Transparent red
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Bright red
            }
        } else {  // ENEMY_FYGAR
            // Fygars are green
            if (enemy->is_ghosting) {
                SDL_SetRenderDrawColor(renderer, 0, 100, 0, 180);  // Transparent green
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Bright green
            }
        }
        
        SDL_RenderFillRect(renderer, &enemy_rect);
        
        // Draw facing indicator (small circle)
        SDL_FRect indicator;
        indicator.w = TILE_SIZE / 4;
        indicator.h = TILE_SIZE / 4;
        
        // Position based on direction
        switch(enemy->facing) {
            case DIR_UP:
                indicator.x = x + TILE_SIZE / 2 - TILE_SIZE / 8;
                indicator.y = y + 4;
                break;
            case DIR_DOWN:
                indicator.x = x + TILE_SIZE / 2 - TILE_SIZE / 8;
                indicator.y = y + TILE_SIZE - 4 - TILE_SIZE / 4;
                break;
            case DIR_LEFT:
                indicator.x = x + 4;
                indicator.y = y + TILE_SIZE / 2 - TILE_SIZE / 8;
                break;
            case DIR_RIGHT:
                indicator.x = x + TILE_SIZE - 4 - TILE_SIZE / 4;
                indicator.y = y + TILE_SIZE / 2 - TILE_SIZE / 8;
                break;
        }
        
        // Yellow indicator
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &indicator);
    }
}
```

**Visual Design:**
```
POOKA (not ghosting):
â"Œâ"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"
â"‚                  â"‚
â"‚  â—  BRIGHT RED   â"‚  â† Yellow dot = facing
â"‚                  â"‚
â""â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"˜

POOKA (ghosting):
â"Œâ"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"
â"‚                  â"‚
â"‚  â—  DARK/TRANS   â"‚  â† Darker when in dirt
â"‚      RED         â"‚
â""â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"˜

FYGAR:
â"Œâ"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"
â"‚                  â"‚
â"‚  â—  GREEN        â"‚  â† Yellow dot = facing
â"‚                  â"‚
â""â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"€â"˜
```

### 4. Update main.c

**Add includes:**
```c
#include "enemy.h"
#include <time.h>   // For rand() seeding
#include <stdlib.h>
```

**Add after grid initialization:**
```c
int main(int argc, char *argv[]) {
    // ... SDL initialization ...
    
    // Seed random number generator for enemy AI
    srand(time(NULL));
    
    TileType grid[GRID_HEIGHT][GRID_WIDTH];
    grid_init(grid);
    
    Player player;
    player_init(&player, 10, 2);
    
    // NEW: Create enemies
    Enemy enemies[MAX_ENEMIES];
    int enemy_count = 0;
    
    // Spawn 2 Pookas
    enemy_init(&enemies[enemy_count++], ENEMY_POOKA, 15, 5);
    enemy_init(&enemies[enemy_count++], ENEMY_POOKA, 5, 10);
    
    // Spawn 1 Fygar
    enemy_init(&enemies[enemy_count++], ENEMY_FYGAR, 10, 8);
    
    // ... rest of game loop ...
}
```

**Update the UPDATE section:**
```c
    // ========= UPDATE (game Logic) ===========
    player_update(&player);
    
    // Update all enemies
    for (int i = 0; i < enemy_count; i++) {
        enemy_update(&enemies[i], &player, grid);
        
        // Check collision with player
        if (enemy_collides_with_player(&enemies[i], &player)) {
            printf("Hit by enemy! Game over!\n");
            player.is_alive = false;
            // For now just print, we'll add game over screen later
        }
    }
```

**Update the RENDER section:**
```c
    // ========= RENDER ========================
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_draw_grid(renderer, grid);
    render_draw_enemies(renderer, enemies, enemy_count);  // NEW
    render_draw_player(renderer, &player);
    render_draw_hud(renderer, &player);

    SDL_RenderPresent(renderer);
```

**Important:** Enemies drawn BEFORE player so player appears on top!

### 5. Update Makefile

**Add enemy.c to sources:**
```makefile
SOURCES = main.c grid.c render.c player.c enemy.c
OBJECTS = main.o grid.o render.o player.o enemy.o
HEADERS = types.h grid.h render.h player.h enemy.h
```

**Add enemy.o rule:**
```makefile
enemy.o: enemy.c types.h enemy.h player.h grid.h
	$(CC) $(CFLAGS) -c enemy.c -o enemy.o
```

**Update main.o dependencies:**
```makefile
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o main.o
```

## Testing

### Compile and Run
```bash
make clean && make
./digdug
```

### What to Test

**1. Enemies Spawn**
- [ ] Red squares (Pookas) appear
- [ ] Green square (Fygar) appears
- [ ] Yellow dots show facing direction

**2. Enemy Movement**
- [ ] Enemies move towards you
- [ ] They path around obstacles
- [ ] They slow down in dirt (darker color)
- [ ] They speed up in tunnels (brighter color)

**3. AI Behavior**
- [ ] Enemies chase you when you move
- [ ] They try to catch up
- [ ] They pick reasonable paths
- [ ] They don't get stuck forever

**4. Collision Detection**
- [ ] "Hit by enemy! Game over!" prints
- [ ] Game continues (for now)

**5. Ghosting Visual**
- [ ] Enemies become semi-transparent in dirt
- [ ] They're solid in tunnels

## Understanding the AI

### Simple Chase Algorithm
```
1. Where is the player?
2. Am I more left/right or up/down from them?
3. Move in the direction that closes the gap
4. If blocked, try a random other direction
```

**Why this works:**
- Simple to implement
- Gives reasonable behavior
- Enemies feel threatening
- Not perfect = not frustrating

**Example:**
```
Player at (10, 5)
Enemy at (6, 8)

dx = 10 - 6 = 4  (player is right)
dy = 5 - 8 = -3  (player is up)

abs(4) > abs(3)  (horizontal difference larger)
â†' Move RIGHT
```

### Cooldown System
```c
// Fast enemies (in tunnels)
move_cooldown = 6;  // ~100ms at 60 FPS

// Slow enemies (in dirt)
move_cooldown = 12; // ~200ms at 60 FPS
```

**Why different speeds?**
- Tunnels = enemy territory = they're fast
- Dirt = you're digging = you have advantage
- Creates strategic depth

### Ghosting Mechanic
```c
if (tile == TILE_DIRT) {
    enemy->is_ghosting = true;
    // Slower movement
    // Darker/transparent visual
}
```

**Why ghost through dirt?**
- Original Dig Dug behavior
- Prevents enemies from getting stuck
- Creates tension (they can come from anywhere!)
- Balances with player's digging advantage

## Experiments

### 1. More Enemies
```c
// Spawn 5 Pookas and 3 Fygars
for (int i = 0; i < 5; i++) {
    int col = rand() % GRID_WIDTH;
    int row = 5 + rand() % 10;
    enemy_init(&enemies[enemy_count++], ENEMY_POOKA, col, row);
}

for (int i = 0; i < 3; i++) {
    int col = rand() % GRID_WIDTH;
    int row = 5 + rand() % 10;
    enemy_init(&enemies[enemy_count++], ENEMY_FYGAR, col, row);
}
```

### 2. Different AI Behaviors
```c
// Fygar: More aggressive (faster cooldown)
if (enemy->type == ENEMY_FYGAR) {
    enemy->move_cooldown = 4;
} else {
    enemy->move_cooldown = 6;
}

// Pooka: Random wandering sometimes
if (rand() % 100 < 20) {  // 20% chance
    Direction random_dir = rand() % 4;
    enemy_try_move(enemy, random_dir, grid);
}
```

### 3. Smarter Pathfinding
```c
// A* algorithm
// Breadth-first search
// Line-of-sight checking
// Waypoint system
```

### 4. Enemy States
```c
typedef enum {
    ENEMY_STATE_ROAMING,
    ENEMY_STATE_CHASING,
    ENEMY_STATE_FLEEING,
    ENEMY_STATE_STUNNED
} EnemyState;
```

### 5. Difficulty Scaling
```c
// Increase enemy speed over time
int game_time = SDL_GetTicks() / 1000;  // Seconds
int speed_bonus = game_time / 30;       // Every 30 seconds
move_cooldown = max(2, 6 - speed_bonus);
```

## Common Issues

### Enemies Don't Move
```c
// Debug AI
printf("Enemy at (%d,%d), Player at (%d,%d)\n",
       enemy->col, enemy->row, player->col, player->row);
printf("Preferred direction: %d, Cooldown: %d\n",
       preferred, enemy->move_cooldown);
```

### Enemies Get Stuck
```c
// Add more alternatives
Direction all_dirs[4] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
for (int i = 0; i < 4; i++) {
    if (enemy_try_move(enemy, all_dirs[i], grid)) {
        break;
    }
}
```

### Enemies Too Fast/Slow
```c
// Tune cooldown values
enemy->move_cooldown = 8;   // Try 4, 6, 8, 10, 12
```

### Collision Feels Wrong
```c
// Currently: exact tile match required
// Could use: bounding box overlap
// Could use: distance threshold
```

### Enemies Ignore Player
```c
// Check get_direction_to() logic
printf("dx=%d, dy=%d, preferred=%d\n", dx, dy, preferred);
```

## Advanced Concepts

### Entity Management Pattern
```c
// Array of entities
Enemy enemies[MAX_ENEMIES];
int enemy_count;

// Spawn
if (enemy_count < MAX_ENEMIES) {
    enemy_init(&enemies[enemy_count++], ...);
}

// Update all
for (int i = 0; i < enemy_count; i++) {
    enemy_update(&enemies[i], ...);
}

// Remove dead
for (int i = 0; i < enemy_count; i++) {
    if (!enemies[i].is_alive) {
        // Swap with last, decrease count
        enemies[i] = enemies[enemy_count - 1];
        enemy_count--;
        i--;  // Recheck this index
    }
}
```

### State Machines for AI
```c
switch(enemy->state) {
    case ENEMY_STATE_ROAMING:
        // Random movement
        if (can_see_player) {
            enemy->state = ENEMY_STATE_CHASING;
        }
        break;
        
    case ENEMY_STATE_CHASING:
        // Chase player
        if (player_used_powerup) {
            enemy->state = ENEMY_STATE_FLEEING;
        }
        break;
        
    case ENEMY_STATE_FLEEING:
        // Run away
        if (powerup_expired) {
            enemy->state = ENEMY_STATE_CHASING;
        }
        break;
}
```

### Separation of Concerns
```
enemy.c:
  - AI logic
  - Movement logic
  - State management

render.c:
  - How enemies look
  - Animation (later)

main.c:
  - Spawning enemies
  - Collision checks
  - Game rules
```

## Key Takeaways

- Enemies are just entities with AI update functions  
- Arrays manage multiple entities efficiently  
- Simple chase AI feels intelligent enough  
- Cooldowns create different enemy personalities  
- Visual feedback (ghosting) improves game feel  
- Forward declarations avoid circular includes  
- Random alternatives prevent enemies getting stuck  
- Drawing order matters (enemies before player)  

## Next Steps

**Step 7: Air Pump Weapon**
- Hold space to inflate enemies
- Enemies expand then pop
- Score system
- Victory condition

**Step 8: Rocks and Physics**
- Rocks fall when dirt removed underneath
- Crush enemies (and player!)
- Chain reactions

**Step 9: Game States and Polish**
- Menu screen
- Game over screen
- Score display
- Level progression
- Sound effects

## Architecture Review

```
main.c
  â†" calls
player.c â†' Updates player state
enemy.c  â†' Updates enemy states (with player reference)
grid.c   â†' Provides world data
render.c â†' Draws everything

Data flow:
  Input â†' Player moves
         â†' Enemies react to player position
         â†' Collision detection
         â†' Render everything
```

Congratulations! You now have enemies chasing you through the tunnels. Time to fight back! đŸ'Ș
