#include "neuralNetwork/network_Utils/logicalBrain.h"
#include "neuralNetwork/network_Utils/mnistUtils.h"
#include "neuralNetwork/network_Utils/struct.h"
#include "GUI/handleUtils.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/Image_Clean/Image_Clean.h"
#include "neuralNetwork/network_Utils/createNetwork.h"
#include "neuralNetwork/network_Utils/saveLoadNetwork.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <time.h>
#include <stdlib.h>

#define NETWORK_PATH "../digitsTrain.txt"

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

void findAllDigits(GridCell** gridCells, int count, Flag* flags){


    if(flags[0].value == 1){
        printf("🚀 Starting to load neural network.\n");
    }

    NeuralNetwork* net = loadNetwork(NETWORK_PATH);

    if(flags[0].value == 1){
        printf("✅ Success to load neural network.\n");
        printNetworkSpec(net);
    }

    double input[784];

    for (int img_i = 0; img_i < count; img_i++)
    {
        GridCell* currentGridCell = gridCells[img_i];

        if(currentGridCell->isDigit != 1){
            continue;
        }

        for (int px_i = 0; px_i < 784; px_i++)
        {
            Uint32* pixels = currentGridCell->image->pixels;

            input[px_i] = (double)getPixelGrayScale(pixels[px_i])/255.0;
        }

        forwardPropagation(net, input);


        Layer* lL = &net->layers[net->nb_layers-1];

        int digitRecognised = 0;
        for (int i = 1; i < lL->nb_neurons; i++) {
            if (lL->neurons[i].output >
                    lL->neurons[digitRecognised].output) {
                digitRecognised = i;
            }
        }

        currentGridCell->label = digitRecognised;
    }

    destroyNetwork(net);
}