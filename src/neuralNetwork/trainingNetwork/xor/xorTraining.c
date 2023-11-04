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
            int result = inputA != inputB;

            net->layers[0].neurons[0].output = inputA;
            net->layers[0].neurons[1].output = inputB;

            hiddenPropagation(net);

            double output[2];
            outputPropagation(net, output);

            int digitRecognised = 0;
            for (int i = 1; i < net->layers[0].nb_neurons; i++) {
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
            backPropagation(net, output, trueProbs, tP.learningRate);
        }

        if(flags[0].value == 1){
            double accuracy = (double)correctPredictions / tP.batchSize * 100.0;
            printf("Epoch %d, Correct answers : %.2f%%\n",
                epoch_i + 1, accuracy);
        }
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