CC = gcc -I../../include
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags gtk+-3.0)
LDLIBS = -lm $(shell pkg-config --libs gtk+-3.0)

SRC_DIR = ./

SRC = $(shell find $(SRC_DIR) -type f -name "*.c")


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

clean-obj:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")