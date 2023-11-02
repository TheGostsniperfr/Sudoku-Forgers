#ifndef SAVE_LOAD_NETWORK_H
#define SAVE_LOAD_NETWORK_H_H

#include "neuralNetwork/network_Utils/struct.h"


void saveNetwork(char* filename, NeuralNetwork* net);
NeuralNetwork* loadNetwork(char* filename);

#endif