#ifndef DIGITS_TRAINING_H
#define DIGITS_TRAINING_H

#include "neuralNetwork/network_Utils/struct.h"

void digitTraining(
    NeuralNetwork* net,
    TrainingPara tP,
    char* filename,
    Flag* flags);

#endif