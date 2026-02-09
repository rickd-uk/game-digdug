# Step 5: Enhanced Digging Mechanics

**Goal:** Make digging feel satisfying with multiple gameplay enhancements.

## What We're Building

In Step 4, we could only move through existing tunnels. Now we'll add:
1. **Automatic digging** - Moving into dirt converts it to tunnel
2. **Dirt counter** - Track your progress
3. **Movement timing** - Digging feels slower than running
4. **Directional rules** - Can't dig straight up (physics!)

## New Concepts

- **Game state management** - Tracking player statistics
- **Frame-based timing** - Cooldown/delay systems
- **Game feel** - Making actions feel weighty and deliberate
- **Rule-based mechanics** - Different behaviors based on context

## Step-by-Step Implementation

### 1. Add Player State Tracking

**Modify `player.h`:**
```c
typedef struct {
  int col;
  int row;
  Direction facing;
  bool is_alive;
  int dirt_dug;        // NEW: Track total dirt dug
  int move_slowdown;   // NEW: Frames until next move allowed
} Player;
```

**Why?**
- `dirt_dug` - Game state we can use for scoring, achievements, progression
- `move_slowdown` - Creates deliberate, weighty movement feel

### 2. Initialize New Fields

**Update `player_init()` in `player.c`:**
```c
void player_init(Player *player, int start_col, int start_row) {
  player->col = start_col;
  player->row = start_row;
  player->facing = DIR_RIGHT;
  player->is_alive = true;
  player->dirt_dug = 0;        // NEW
  player->move_slowdown = 0;   // NEW
}
```

### 3. Implement Digging Logic

**Replace `player_move()` in `player.c`:**
```c
bool player_move(Player *player, Direction dir,
                 TileType grid[GRID_HEIGHT][GRID_WIDTH]) {
  
  // Check slowdown - can't move yet
  if (player->move_slowdown > 0) {
    return false;
  }
  
  int new_col = player->col;
  int new_row = player->row;

  // calc new pos based on dir
  switch (dir) {
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

  // boundary check
  if (new_col < 0 || new_col >= GRID_WIDTH || new_row < 0 ||
      new_row >= GRID_HEIGHT) {
    return false;
  }

  // check destination tile
  TileType destination = grid_get_tile(grid, new_row, new_col);
  
  // RULE: Can't dig upward through dirt (gravity!)
  if (destination == TILE_DIRT && dir == DIR_UP) {
    return false;
  }
  
  // DIG LOGIC: Convert dirt to tunnel before moving
  bool just_dug = false;
  if (destination == TILE_DIRT) {
    grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
    destination = TILE_TUNNEL;
    just_dug = true;
    player->dirt_dug++;  // Track it!
  }
  
  // now check if we can walk there
  if (!can_move_to(destination)) {
    return false; // blocked by rock
  }

  // move is valid - update position
  player->col = new_col;
  player->row = new_row;
  player->facing = dir;
  
  // Set movement slowdown based on what we did
  if (just_dug) {
    player->move_slowdown = 8;  // Slower when digging (8 frames)
  } else {
    player->move_slowdown = 3;  // Faster in tunnels (3 frames)
  }

  return true;
}
```

**Key Design Decisions:**

```c
if (player->move_slowdown > 0) {
  return false;
}
```
- Prevents movement until cooldown expires
- Called every frame, but only moves when ready

```c
if (destination == TILE_DIRT && dir == DIR_UP) {
  return false;
}
```
- Realistic physics - you can't dig upward through dirt!
- Can still move UP through existing tunnels

```c
bool just_dug = false;
if (destination == TILE_DIRT) {
  grid_set_tile(grid, new_row, new_col, TILE_TUNNEL);
  destination = TILE_TUNNEL;
  just_dug = true;
  player->dirt_dug++;
}
```
- Convert dirt before the walkability check
- Track every dirt tile dug
- Flag tells us to apply dig slowdown

```c
if (just_dug) {
  player->move_slowdown = 8;  // Digging
} else {
  player->move_slowdown = 3;  // Running
}
```
- Digging feels heavy (8 frames ≈ 133ms at 60 FPS)
- Running feels responsive (3 frames ≈ 50ms at 60 FPS)
- Creates satisfying game feel difference

### 4. Add Frame Update System

**Add to `player.c`:**
```c
void player_update(Player *player) {
  if (player->move_slowdown > 0) {
    player->move_slowdown--;
  }
}
```

**Declare in `player.h`:**
```c
void player_update(Player *player);
```

**Why a separate update function?**
- Separates concerns: movement logic vs. timing logic
- Called every frame, even when not moving
- Foundation for future features (animations, AI, etc.)

### 5. Integrate into Game Loop

**Modify `main.c` UPDATE section:**
```c
    // ========= UPDATE (game Logic) ===========
    player_update(&player);  // NEW: Update slowdown timer
```

**Game loop now looks like:**
```
Input → Check slowdown → Move if allowed → Update slowdown → Render
```

### 6. Create Visual Feedback (HUD)

**Add to `render.c`:**
```c
void render_draw_hud(SDL_Renderer *renderer, Player *player) {
  // Draw a simple dirt counter as colored bars
  // Each 10 dirt = one bar
  int bars = player->dirt_dug / 10;
  
  SDL_FRect bar;
  bar.w = 20;
  bar.h = 10;
  bar.y = 5;
  
  for (int i = 0; i < bars && i < 30; i++) {
    bar.x = 5 + (i * 22);
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // dirt brown
    SDL_RenderFillRect(renderer, &bar);
  }
}
```

**Declare in `render.h`:**
```c
void render_draw_hud(SDL_Renderer *renderer, Player *player);
```

**Call in `main.c` RENDER section:**
```c
    // Draw player on top
    render_draw_player(renderer, &player);
    
    // Draw HUD (dirt counter)
    render_draw_hud(renderer, &player);  // NEW
```

**Design notes:**
- Each bar = 10 dirt tiles
- Max 30 bars (300 dirt) prevents HUD overflow
- Uses dirt brown color for thematic consistency
- Simple visual progress indicator

## Testing Your Changes

### Compile and Run
```bash
make clean && make
./digdug
```

### Test Cases

**1. Basic Digging**
- Move down/left/right into dirt
- ✅ Should dig through and move
- ✅ Light brown → dark brown conversion
- ✅ Bars appear at top

**2. Movement Speed Difference**
- Dig through dirt, then move through the tunnel you just created
- ✅ Digging should feel noticeably slower
- ✅ Running through tunnels should feel faster

**3. Upward Digging Block**
- Try moving UP into dirt above you
- ✅ Should be blocked
- ✅ Can still move UP through existing tunnels

**4. Rock Collision**
- Move to row 5, col 10 where the rock is
- ✅ Should bounce off the rock
- ✅ Rock shouldn't turn into tunnel

**5. Dirt Counter**
- Dig around and watch the bars
- ✅ Each 10 dirt = new brown bar
- ✅ Bars appear from left to right

**6. Boundary Testing**
- Try moving off screen edges
- ✅ Should be blocked at boundaries

## Common Issues

### "Player moves through dirt instantly"
- Check that `player_update()` is called in main loop
- Verify `move_slowdown` is being set correctly
- Add printf to debug: `printf("slowdown: %d\n", player->move_slowdown);`

### "Can't move at all"
- Check initial `move_slowdown = 0` in `player_init()`
- Make sure `player_update()` decrements the counter

### "Can dig upward"
- Verify the upward digging check is BEFORE the dirt conversion
- Check: `if (destination == TILE_DIRT && dir == DIR_UP)`

### "Bars don't show up"
- Ensure `render_draw_hud()` is called AFTER `SDL_RenderClear()`
- Check HUD is drawn LAST (on top of everything)
- Verify dirt_dug is actually incrementing

## What We Learned

### Game Feel Concepts
- **Timing creates weight** - Slow actions feel powerful
- **Context-sensitive speed** - Different speeds for different actions
- **Visual feedback** - Players need to see their progress

### Programming Patterns
- **Frame-based timing** - Count down frames for delays
- **State tracking** - Store game statistics in entities
- **Separation of concerns** - Update logic separate from movement logic

### Design Patterns
```
Frame-based Update Pattern:
1. Set counter when action happens
2. Decrement counter every frame
3. Allow action when counter hits 0
```

## Next Steps

With digging working, we have the core game mechanic! Next up:

**Step 6: Enemies and AI**
- Pooka and Fygar enemies
- Basic AI pathfinding
- Collision with player

**Future Enhancements:**
- Sound effects when digging
- Particle effects (dirt spray)
- Score points for digging
- Different terrain types (hard dirt, soft dirt)
- Power-ups hidden in dirt

## Code Architecture Review

```
Input
  ↓
Check slowdown (can we move?)
  ↓
Calculate new position
  ↓
Check if destination is dirt
  ├─ Yes → Dig it (dirt→tunnel)
  └─ No → Continue
  ↓
Check if destination is walkable
  ├─ Yes → Move + Set slowdown
  └─ No → Blocked
  ↓
Update slowdown timer (every frame)
  ↓
Render
```

## Key Takeaways

✅ Digging is just **modifying the grid** before moving  
✅ **Cooldown systems** create deliberate game feel  
✅ **Directional rules** add interesting constraints  
✅ **Visual feedback** keeps players engaged  
✅ **Frame-based timing** is the foundation of game loops  
✅ **State tracking** enables scoring and progression

Congratulations! You now have a working digging mechanic with satisfying game feel!
