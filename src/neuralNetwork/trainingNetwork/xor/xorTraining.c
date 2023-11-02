#include "neuralNetwork/network_Utils/logicalBrain.h"
#include "GUI/handleUtils.h"
#include "neuralNetwork/network_Utils/networkUtils.h"
#include "neuralNetwork/network_Utils/saveLoadNetwork.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>



void xorTraining(NeuralNetwork* net, TrainingPara tP, char* filename, Flag* flags){
    if(flags[0].value == 1){
        printf("🚀 Starting xor training.\n");
    }

    if(net->layers[0].nb_neurons != 2 ||
        net->layers[net->nb_layers-1].nb_neurons != 1)
    {
        errx(EXIT_FAILURE, "❌ Invalid network settings for xor training.\n");
    }

    //Get Xor data set
    double xorDataSet[] =
    {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0
    };

    double xorDataSetExpected[] =
    {
        0.0,
        1.0,
        1.0,
        0.0
    };

    int sizeDataSet = 4;
    //think to compare to tP.batchSize

    double errorRate = 0.0;

    for (int epoch_i = 1; epoch_i <= tP.nbEpoch; epoch_i++)
    {
        if(flags[0].value == 1){
            printf("Epoch = %d\n", epoch_i);
        }

        for (int i = 0; i < sizeDataSet; i++)
        {
            double input[] = { xorDataSet[2 * i], xorDataSet[2 * i + 1] };
            double expected[] = { xorDataSetExpected[i] };

            forwardPropagation(net, input);
            errorRate = backPropagation(net, expected);
            gradientDescent(net, tP.learningRate);

            if(flags[0].value == 1){
                printf("Input : %f, %f | Output : %f |Expected : %f\n", input[0], input[1],
                    net->layers[net->nb_layers-1].neurons[0].output, expected[0]);
            }
        }
    }

    if(flags[0].value == 1){
        printf("Error rate : %f\n", errorRate);
    }



    if(flags[1].value == 1){

        saveNetwork(filename, net);
        if(flags[0].value == 1){
                printf("💾 Success to save network.\n");
        }
    }

    if(flags[0].value == 1){
        printf("✅ Success of xor training.\n");
    }
}