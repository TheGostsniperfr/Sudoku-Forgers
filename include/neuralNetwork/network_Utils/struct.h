#ifndef BUILD_NETWORK_H
#define BUILD_NETWORK_H

#include "SDL2/SDL.h"
#include "preProcessing/CaseDetection/case_detection.h"

//---------------------------
//
//      NETWORK STRUCTS :
//
//---------------------------

typedef struct Neuron {
    double *weights;
    double bias;
    double output;
    double z;
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
    int batchSize; //number of images uses in the dataset
    int miniBatchSize; //Size of each mini batch
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


//---------------------------
//
//      DATA SET STRUCTS :
//
//---------------------------

typedef struct ImgContainer{
    SDL_Surface* img;
    int label;
} ImgContainer;

typedef struct MiniBatch{
    int startIndex;
    int length;
} MiniBatch;

typedef struct Batch{
    ImgContainer* imgContainer; // list of image (dataset)
    MiniBatch* miniBatchs; //list of mini batch
    int nbMiniBatch;
} Batch;

//---------------------------
//
//      HANDLER STRUCT :
//
//---------------------------

typedef struct AllStepResult{
    GridCell* gridCells;
    SDL_Surface* binarizedImg;
    SDL_Surface* homographyImg;
    SDL_Surface* gridImg;
} AllStepResult;

#endif