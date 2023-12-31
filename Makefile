CC = gcc -I include
CFLAGS = $(shell sdl2-config --cflags) \
	$(shell pkg-config --cflags gtk+-3.0) -Wall -Wextra -g

LDLIBS = $(shell sdl2-config --libs)  \
	$(shell pkg-config --libs gtk+-3.0) -lSDL2_image -lSDL2_ttf -lm
SRC_DIR = ./src

SRC = $(shell find $(SRC_DIR) -type f -name "*.c" )

SRC := $(filter-out $(SRC_DIR)/preProcessing/preProcessing.c, $(SRC))
SRC := $(filter-out $(SRC_DIR)/sudokuSolver/solver.c, $(SRC))
SRC := $(filter-out $(SRC_DIR)/neuralNetwork/network.c, $(SRC))

OBJ = ${SRC:.c=.o}
TARGET = main

all: $(TARGET) solver network pre
	make dataset -C $(SRC_DIR)/neuralNetwork

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@



solver:
	make -C $(SRC_DIR)/sudokuSolver

network:
	make -C $(SRC_DIR)/neuralNetwork

pre:
	make -C $(SRC_DIR)/preProcessing




clean:
	make -C $(SRC_DIR)/sudokuSolver clean
	make -C $(SRC_DIR)/neuralNetwork clean
	make -C $(SRC_DIR)/preProcessing clean

	rm -f src/GUI/app
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")
	rm -f $(shell find ./ -type f -name "*.jpg")
	rm -f $(shell find ./ -type f -name "*.result")
	rm -f $(shell find ./src/GUI/tmp/ -type f -name "*.jpg")
	rm -f src/GUI/tmp/grid

	rm -f $(TARGET)

	clear


clean-obj:
	make -C $(SRC_DIR)/sudokuSolver clean-obj
	make -C $(SRC_DIR)/preProcessing clean-obj
	make -C $(SRC_DIR)/neuralNetwork clean-obj

	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")

	clear
