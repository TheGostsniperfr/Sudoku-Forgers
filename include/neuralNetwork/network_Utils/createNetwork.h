#ifndef CREATE_NETWORK_H
#define CREATE_NETWORK_H

#include "neuralNetwork/network_Utils/struct.h"

#include <stdio.h>
#include <stdlib.h>


void destroyNetwork(NeuralNetwork* net);
NeuralNetwork* createNetwork(NetworkPara netPara);


#endif