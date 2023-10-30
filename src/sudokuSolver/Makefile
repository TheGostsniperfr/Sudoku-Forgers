CC = gcc -I../../include
CFLAGS = $(shell sdl2-config --cflags) -Wall -Wextra -std=c99 -O1
LDLIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lm

SRC_DIR = ./

SRC = $(shell find $(SRC_DIR) -type f -name "*.c")

SRC += ../preProcessing/SDL_Function/sdlFunction.c

OBJ = ${SRC:.c=.o}
TARGET = solver

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")
	rm -f $(TARGET)

	rm -f ../preProcessing/SDL_Function/sdlFunction.o




clean-obj:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")