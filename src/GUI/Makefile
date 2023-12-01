CC = gcc -I../../include
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags gtk+-3.0) -g $(shell sdl2-config --cflags)
LDLIBS = -lm $(shell pkg-config --libs gtk+-3.0) $(shell sdl2-config --libs) -lSDL2_image

SRC_DIR = ./

SRC = $(shell find $(SRC_DIR) -type f -name "*.c")
SRC += ../preProcessing/SDL_Function/sdlFunction.c
SRC += ../preProcessing/Rotate/rotate.c
SRC += ../preProcessing/Matrix/mat.c


OBJ = ${SRC:.c=.o}
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")
	rm -f $(TARGET)
	rm -f ../preProcessing/SDL_Function/sdlFunction.o
	rm -f ../preProcessing/Rotate/rotate.o
	rm -f ../preProcessing/Matrix/mat.o

clean-obj:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")