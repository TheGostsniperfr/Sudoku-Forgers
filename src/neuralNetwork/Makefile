CC = gcc -I../../include
CFLAGS = -Wall -Wextra
LDLIBS = -lm

SRC_DIR = ./

SRC = $(shell find $(SRC_DIR) -type f -name "*.c")

OBJ = ${SRC:.c=.o}
TARGET = network

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")
	rm -f $(TARGET)
	rm -f xorTrain.txt
	rm -f digitTrain.txt






clean-obj:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")