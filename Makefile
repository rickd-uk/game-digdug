# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = $(shell pkg-config --libs sdl3)
CFLAGS += $(shell pkg-config --cflags sdl3)

# Target executable
TARGET = digdug

# Source files
SOURCES = main.c grid.c render.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

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

.PHONY: all clean run info
