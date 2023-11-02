#ifndef XOR_TRAINING_H
#define XOR_TRAINING_H

#include "neuralNetwork/network_Utils/struct.h"


void xorTraining
(
    NeuralNetwork* net,
    TrainingPara tP,
    char* filename,
    Flag* flags
);


#endif