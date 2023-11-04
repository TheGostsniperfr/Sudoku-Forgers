#ifndef LOGICAL_BRAIN_H
#define LOGICAL_BRIAN_H

#define true 1
#define false 0


#include "neuralNetwork/network_Utils/struct.h"



void initializeNetwork(NeuralNetwork *net);
void hiddenPropagation(NeuralNetwork *net);
void outputPropagation(NeuralNetwork *net,  double *output);

void backPropagation(NeuralNetwork *net, double *predicted_probs,
    double *trueProbs, double learningRate);



#endif