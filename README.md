# Dig Dug Clone

A step-by-step recreation of the classic 1982 arcade game Dig Dug, built from scratch using SDL3 and C. This project teaches fundamental game programming concepts through hands-on development.

## About the Game

Dig Dug is a maze arcade game where you tunnel through underground dirt to hunt monsters. Defeat enemies by inflating them with your air pump until they explode. Features Pookas (red balloon monsters) and Fygars (green fire-breathing dragons). Rocks can fall and crush enemies. The deeper you dig, the more points you earn!

## Current Status

[x] **Step 1**: SDL3 window and game loop  
[x] **Step 2**: Drawing rectangles (tile building blocks)  
[x] **Step 3**: Full tile grid system + modular code structure  
[x] **Step 4**: Player character with keyboard movement  
[x] **Step 5**: Digging mechanics with movement timing  
[x] **Step 6**: Enemies and AI (Pookas and Fygars)  
[ ] **Step 7**: Air pump weapon  
[ ] **Step 8**: Rocks and physics  
[ ] **Step 9**: Score and game states  

## Building and Running

### Requirements
- SDL3 (3.4.0+)
- GCC or compatible C compiler
- Make

### Compilation
```bash
make clean
make
./digdug
```

### Manual Compilation
```bash
gcc -Wall -Wextra -std=c11 -g -c main.c -o main.o
gcc -Wall -Wextra -std=c11 -g -c grid.c -o grid.o
gcc -Wall -Wextra -std=c11 -g -c render.c -o render.o
gcc -Wall -Wextra -std=c11 -g -c player.c -o player.o
gcc -Wall -Wextra -std=c11 -g -c enemy.c -o enemy.o
gcc main.o grid.o render.o player.o enemy.o -lSDL3 -o digdug
```

## Controls

- **Arrow Keys**: Move Dig Dug
- **ESC**: Quit

## Project Structure
```
digdug/
â"œâ"€â"€ main.c              # Game loop and SDL initialization
â"œâ"€â"€ types.h             # Shared constants and type definitions
â"œâ"€â"€ grid.h/grid.c       # Grid/world logic
â"œâ"€â"€ render.h/render.c   # All rendering code
â"œâ"€â"€ player.h/player.c   # Player logic and movement
â"œâ"€â"€ enemy.h/enemy.c     # Enemy AI and behavior
â"œâ"€â"€ Makefile            # Build configuration
â"œâ"€â"€ README.md           # This file
â""â"€â"€ docs/               # Step-by-step learning documentation
    â"œâ"€â"€ STEP1.md
    â"œâ"€â"€ STEP2.md
    â"œâ"€â"€ STEP3.md
    â"œâ"€â"€ STEP4.md
    â"œâ"€â"€ STEP5.md
    â"œâ"€â"€ STEP6.md
    â"œâ"€â"€ CONCEPTS.md
    â""â"€â"€ TROUBLESHOOTING.md
```

## Learning Path

This project is designed to teach game development from first principles. Each step builds on the previous one:

1. **Step 1** - SDL3 basics, game loop architecture
2. **Step 2** - 2D rendering fundamentals
3. **Step 3** - Tile-based worlds, code organization
4. **Step 4** - Input handling, collision detection, entity management
5. **Step 5** - Digging mechanics, frame-based timing, game feel
6. **Step 6** - Enemy AI, pathfinding, multiple entity management

See the `docs/` folder for detailed step-by-step guides.

## Key Concepts Learned

- SDL3 initialization and cleanup
- Game loop architecture (Input â†' Update â†' Render)
- Event handling (keyboard, window events)
- 2D tile-based rendering
- Grid coordinate systems
- Modular C project organization
- Header guards and compilation
- Structs and enums
- Collision detection
- Entity management with arrays
- Frame-based timing systems
- AI pathfinding algorithms
- Context-sensitive movement (ghosting)
- Multiple enemy types with different behaviors

## Resources

- [SDL3 Documentation](https://wiki.libsdl.org/SDL3/)
- [Original Dig Dug](https://en.wikipedia.org/wiki/Dig_Dug)
- [C Programming Reference](https://en.cppreference.com/)

## License

Educational project - feel free to learn from and modify!
