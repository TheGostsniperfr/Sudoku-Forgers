#ifndef BUILD_NETWORK_H
#define BUILD_NETWORK_H

#include "SDL2/SDL.h"

typedef struct Neuron {
    double *weights;
    double bias;
    double output;
    double delta;
} Neuron;

typedef struct Layer {
    int nb_neurons;
    Neuron *neurons;
} Layer;

typedef struct NeuralNetwork {
    int nb_layers;
    Layer *layers;
} NeuralNetwork;

typedef struct TrainingPara {
    int nbEpoch;
    int batchSize;
    double learningRate;

    //save -> 1, else -> 0
    int saveTraining;

} TrainingPara;

typedef struct NetworkPara{
    int nbNeuronsFirstLayer;
    int nbNeuronsHiddenLayer;
    int nbNeuronsOutputLayer;
    int nbHiddenLayers;
} NetworkPara;

typedef struct ImgContainer{
    SDL_Surface* img;
    int label;
} ImgContainer;

#endif