SRC_DIR = ./src

all: solver network

solver:
	make -C $(SRC_DIR)/sudokuSolver

network:
	make -C $(SRC_DIR)/neuralNetwork



clean:
	make -C $(SRC_DIR)/sudokuSolver clean
	make -C $(SRC_DIR)/neuralNetwork clean
	rm -f $(shell find $(SRC_DIR) -type f -name "*.jpg")
	rm -f $(shell find $(SRC_DIR) -type f -name "*.result")

	clear


clean-obj:
	make -C $(SRC_DIR)/sudokuSolver clean-obj
	make -C $(SRC_DIR)/neuralNetwork clean-obj

	clear
