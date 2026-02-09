# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lSDL3

# Target executable
TARGET = digdug

# Source files - add new .c files here!
SOURCES = main.c grid.c render.c player.c enemy.c
OBJECTS = main.o grid.o render.o player.o enemy.o

# Header dependencies (if you change a .h file, rebuild)
HEADERS = types.h grid.h render.h player.h enemy.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile each .c file to .o file
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o main.o

grid.o: grid.c $(HEADERS)
	$(CC) $(CFLAGS) -c grid.c -o grid.o

render.o: render.c $(HEADERS)
	$(CC) $(CFLAGS) -c render.c -o render.o

player.o: player.c $(HEADERS)
	$(CC) $(CFLAGS) -c player.c -o player.o

enemy.o: enemy.c $(HEADERS)
	$(CC) $(CFLAGS) -c enemy.c -o enemy.o
	
# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned build files"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Show build info (useful for debugging)
info:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "HEADERS: $(HEADERS)"

.PHONY: all clean run info
