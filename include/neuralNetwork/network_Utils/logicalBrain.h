#ifndef LOGICAL_BRAIN_H
#define LOGICAL_BRIAN_H

#define true 1
#define false 0


#include "neuralNetwork/network_Utils/struct.h"



void initializeNetwork(NeuralNetwork *net);
void backPropagation(NeuralNetwork *net, double *predicted_probs,
    double *trueProbs, double learningRate);

void forwardPropagation(NeuralNetwork* net, double* input);




#endif