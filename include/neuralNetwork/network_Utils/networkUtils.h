#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H


#include <stdio.h>
#include <stdlib.h>

#include "neuralNetwork/network_Utils/struct.h"



void printTrainingPara(TrainingPara tP);
void printNetworkPara(NetworkPara netPara);
void printNetworkSpec(NeuralNetwork* net);

Batch* createBatch(int sizeOfDataSet, int nbMiniBatch);
void shuffleMiniBatch(Batch* batch);
void freeBatch(Batch* batch);

#endif