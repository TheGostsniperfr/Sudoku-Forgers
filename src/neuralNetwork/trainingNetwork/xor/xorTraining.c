#include "neuralNetwork/network_Utils/logicalBrain.h"
#include "GUI/handleUtils.h"
#include "neuralNetwork/network_Utils/networkUtils.h"
#include "neuralNetwork/network_Utils/saveLoadNetwork.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void xorTraining(NeuralNetwork* net, TrainingPara tP,
                    char* filename, Flag* flags){
    if(flags[0].value == 1){
        printf
        (
        "################################################\n\n"

        "       🚀 Starting xor training. :\n\n"

        "################################################\n\n"
        );
    }

    if(net->layers[0].nb_neurons != 2 ||
        net->layers[net->nb_layers-1].nb_neurons != 2)
    {
        errx(EXIT_FAILURE, "❌ Invalid network settings for xor training.\n");
    }


    for (int epoch_i = 0; epoch_i < tP.nbEpoch; epoch_i++)
    {
        int correctPredictions = 0;

        for(int batch_i = 0; batch_i < tP.batchSize; batch_i++){
            //generate random xor data input

           //generate a xor data
            double inputA = rand() % 2;
            double inputB = rand() % 2;

            double input[2] = {inputA, inputB};

            int result = inputA != inputB;

            double output[2];

            forwardPropagation(net, input);

            Layer* lL = &net->layers[net->nb_layers-1];
            output[0] = lL->neurons[0].output;
            output[1] = lL->neurons[1].output;


            int digitRecognised = 0;
            for (int i = 1; i < lL->nb_neurons; i++) {
                if (output[i] > output[digitRecognised]) {
                    digitRecognised = i;
                }
            }

            // check if the prediction is correct
            if (digitRecognised == result) {
                correctPredictions++;
            }

            double trueProbs[2] = {0.0};
            trueProbs[result] = 1.0;

            //back propagation
            //backPropagation(net, output, trueProbs, tP.learningRate);
            backPropagation(net, trueProbs, tP.learningRate);

        }

        if(flags[0].value == 1){

            const int bar_width = 50;

            float progress = (float) (epoch_i+1) / tP.nbEpoch;
            int bar_length = progress * bar_width;

            printf("\rProgress: [");
            for (int i = 0; i < bar_length-1; ++i) {
                printf("=");
            }
            printf(">");
            for (int i = bar_length; i < bar_width-1; ++i) {
                printf(" ");
            }
            printf("] %.2f%% | Accuracy : %.2f%%", progress * 100,
                (double)correctPredictions / tP.batchSize * 100.0);

            fflush(stdout);
        }
    }

    printf("\n\n");

    if(flags[0].value == 1){
        printf
        (
        "################################################\n\n"

        "       ✅ Success to train neural network :\n\n"

        "################################################\n\n"
        );
    }

    if(flags[1].value == 1){

        saveNetwork(filename, net);
        if(flags[0].value == 1){
                printf("💾 Success to save network.\n");
        }
    }
}