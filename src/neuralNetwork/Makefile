CC = gcc -I../../include
CFLAGS = $(shell sdl2-config --cflags) -Wall -Wextra -g
LDLIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lm

SRC_DIR = ./

SRC = $(shell find $(SRC_DIR) -type f -name "*.c")

SRC += ../preProcessing/SDL_Function/sdlFunction.c

OBJ = ${SRC:.c=.o}
TARGET = network

DATASET_DIR := ./../../data/
DATA_FILE := train-images.idx3-ubyte
DF_URL := "https://cdn.discordapp.com/attachments/1161944373051785297/"$\
"1173335619557134478/train-images.idx3-ubyte?ex=656394f3&is=65511ff3&hm="$\
"5c28a1b090f5e19ab17f8032c9c2c7ee5319548f88aed3bfd21cb190f2f5ea27&"



LABEL_FILE := train-labels.idx1-ubyte
LF_URL := "https://cdn.discordapp.com/attachments/1161944373051785297/"$\
"1173335618781184011/train-labels.idx1-ubyte?ex=656394f3&is=65511ff3&hm="$\
"07b80342294aeacd91c3e48a851b7fd4d51319fa3a5642d42ad7613a307d50a6&"

all: $(TARGET) dataset

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

dataset: dataset1 dataset2

dataset1:
	@mkdir -p $(DATASET_DIR)
	@if [ ! -f $(DATASET_DIR)$(DATA_FILE) ]; then \
		echo "Downloading $(DATA_FILE) from $(DF_URL) to $(DATASET_DIR)"; \
		curl -o $(DATASET_DIR)$(DATA_FILE) $(DF_URL); \
	else \
		echo "$(DATA_FILE) already exist in $(DATASET_DIR)"; \
	fi

dataset2:
	@mkdir -p $(DATASET_DIR)
	@if [ ! -f $(DATASET_DIR)$(LABEL_FILE) ]; then \
		echo "Downloading $(LABEL_FILE) from $(LF_URL) to $(DATASET_DIR)"; \
		curl -o $(DATASET_DIR)$(LABEL_FILE) $(LF_URL); \
	else \
		echo "$(LABEL_FILE) already exist in $(DATASET_DIR)"; \
	fi

clean:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")
	rm -f $(shell find $(SRC_DIR) -type f -name "*.jpg")
	rm -f $(TARGET)
	rm -f xorTrain.txt
	rm -f digitTrain.txt
	rm -f ../preProcessing/SDL_Function/sdlFunction.o
	#rm -rf $(DATASET_DIR)/$(LABEL_FILE)
	#rm -rf $(DATASET_DIR)/$(DATA_FILE)

clean-obj:
	rm -f $(shell find $(SRC_DIR) -type f -name "*.o")