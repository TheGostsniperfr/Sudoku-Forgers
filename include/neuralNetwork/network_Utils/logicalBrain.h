#ifndef LOGICAL_BRAIN_H
#define LOGICAL_BRIAN_H

#define true 1
#define false 0


#include "neuralNetwork/network_Utils/struct.h"



void initializeNetwork(NeuralNetwork *net);

void forwardPropagation(NeuralNetwork* net, double* input);

void backPropagation(
    NeuralNetwork* net,
    double* expectedVal,
    double learningRate);



#endif