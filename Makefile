SRC_DIR = ./src

all: solver network pre

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
	rm -f $(shell find ./ -type f -name "*.jpg")
	rm -f $(shell find $(SRC_DIR) -type f -name "*.result")

	clear


clean-obj:
	make -C $(SRC_DIR)/sudokuSolver clean-obj
	make -C $(SRC_DIR)/neuralNetwork clean-obj

	clear
