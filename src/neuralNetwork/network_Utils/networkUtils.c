#include "neuralNetwork/network_Utils/logicalBrain.h"
#include "neuralNetwork/network_Utils/mnistUtils.h"
#include "neuralNetwork/network_Utils/struct.h"
#include "GUI/handleUtils.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <stdlib.h>


void printTrainingPara(TrainingPara tP){
    printf
    (
        "################################################\n\n"

        "       📋 Training data spec :\n\n"

        "################################################\n\n"

        "🧮 Number of epoch : %d\n"
        "📦 Batch size : %d\n"
        "🎓 Learning rate : %.2f\n\n"
        "💾 Save training : %s\n\n",

        tP.nbEpoch,
        tP.batchSize,
        tP.learningRate,
        tP.saveTraining == 1 ? "On" : "Off"
    );
}


void printNetworkPara(NetworkPara netPara){
        printf
    (
        "################################################\n\n"

        "       ⚙️  Neural network parameters :\n\n"

        "################################################\n\n"

        "⬅️ Input layer :\n"
        "       🧠 Number of neurons : %d\n\n"

        "↔️ Hidden layer :\n"
        "       📊 Number of layers : %d\n"
        "       🧠 Number of neurons per layer : %d\n\n"

        "➡️ Output layer :\n"
        "       🧠 Number of neurons : %d\n\n",

        netPara.nbNeuronsFirstLayer,
        netPara.nbHiddenLayers,
        netPara.nbNeuronsHiddenLayer,
        netPara.nbNeuronsOutputLayer
    );
}

void printNetworkSpec(NeuralNetwork* net){
        printf
    (
        "################################################\n\n"

        "       ⚙️ Neural network parameters :\n\n"

        "################################################\n\n"

        "⬅️ Input layer :\n"
        "       🧠 Number of neurons : %d\n\n"

        "↔️ Hidden layer :\n"
        "       📊 Number of layers : %d\n"
        "       🧠 Number of neurons per layer : %d\n\n"

        "➡️ Output layer :\n"
        "       🧠 Number of neurons : %d\n\n",

        net->layers[0].nb_neurons,
        net->nb_layers - 1,
        net->nb_layers > 2 ? net->layers[1].nb_neurons : 0,
        net->layers[net->nb_layers-1].nb_neurons
    );
}

void freeBatch(Batch* batch){
    free(batch->miniBatchs);
    free(batch);
}

Batch* createBatch(int sizeOfDataSet, int nbMiniBatch){
    Batch* batch = calloc(1, sizeof(Batch));

    batch->imgContainer = getDataSet(sizeOfDataSet);
    batch->miniBatchs = calloc(nbMiniBatch, sizeof(MiniBatch));

    batch->nbMiniBatch = nbMiniBatch;

    for (int i = 0; i < nbMiniBatch; i++)
    {
        batch->miniBatchs[i].length = sizeOfDataSet / batch->nbMiniBatch;
        batch->miniBatchs[i].startIndex = batch->miniBatchs[i].length
                                          * i;
    }

    return batch;
}

void shuffleMiniBatch(Batch* batch){
    srand(time(NULL));

    for (int i = 0; i < batch->nbMiniBatch; i++)
    {
        int indexToReverse = rand() % batch->nbMiniBatch;

        MiniBatch tmp = batch->miniBatchs[i];
        batch->miniBatchs[i] = batch->miniBatchs[indexToReverse];
        batch->miniBatchs[indexToReverse] = tmp;
    }
}


double* SdlToMatrix(SDL_Surface* img){
    if(img->w * img->h != 784){
        errx(EXIT_FAILURE, "Size of img not valid.");
    }

    double* imgMat = calloc(784, sizeof(double));

    for (int px_i = 0; px_i < 784; px_i++)
    {
        Uint32* pixels = img->pixels;

        imgMat[px_i] = (double)getPixelGrayScale(pixels[px_i])/255.0;
    }

    return imgMat;
}