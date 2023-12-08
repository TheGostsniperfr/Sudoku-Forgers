#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H


#include <stdio.h>
#include <stdlib.h>

#include "neuralNetwork/network_Utils/struct.h"
#include "preProcessing/Image_Clean/Image_Clean.h"
#include "GUI/handleUtils.h"



void printTrainingPara(TrainingPara tP);
void printNetworkPara(NetworkPara netPara);
void printNetworkSpec(NeuralNetwork* net);

Batch* createBatch(int sizeOfDataSet, int nbMiniBatch, int* realSize);
void shuffleMiniBatch(Batch* batch);
void freeBatch(Batch* batch);

GridCell* findAllDigits(GridCell* gridCells, int count, Flag* flags);


#endif