#include "neuralNetwork/network_Utils/logicalBrain.h"
#include "GUI/handleUtils.h"
#include "neuralNetwork/network_Utils/networkUtils.h"
#include "neuralNetwork/network_Utils/saveLoadNetwork.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void digitTraining(NeuralNetwork* net, TrainingPara tP,
                    char* filename, Flag* flags){
    if(flags[0].value == 1){
        printf
        (
        "################################################\n\n"

        "       🚀 Starting digit training. :\n\n"

        "################################################\n\n"
        );
    }

    if(net->layers[0].nb_neurons != 784 ||
        net->layers[net->nb_layers-1].nb_neurons != 10)
    {
        errx(EXIT_FAILURE, "❌ Invalid network settings for digit training.\n");
    }

    //load data set :
    Batch* batch = createBatch(tP.batchSize, 100);

    printf("batch->length : %d\n", batch->nbMiniBatch);
    for (int i = 0; i < batch->nbMiniBatch; i++)
    {
        printf("batch->miniBatchs[i].length : %d\n", batch->miniBatchs[i].length);

        for (int j = 0; j < batch->miniBatchs[i].length; j++)
        {
            printf("Label : %d\n",  batch->imgContainer[batch->miniBatchs[i].startIndex].label);

        }

    }



    for (int epoch_i = 0; epoch_i < tP.nbEpoch; epoch_i++)
    {
        shuffleMiniBatch(batch);

        int correctPredictions = 0;

        for(int batch_i = 0; batch_i < batch->nbMiniBatch; batch_i++){
            MiniBatch* miniBatch = &batch->miniBatchs[batch_i];
            for (int img_i = 0; img_i < miniBatch->length; img_i++)
            {
                ImgContainer* currentC = &batch->imgContainer[miniBatch->startIndex + img_i];

                double input[784];
                for (int px_i = 0; px_i < 784; px_i++)
                {
                    Uint32* pixels = currentC->img->pixels;

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

                // check if the prediction is correct
                if (digitRecognised == currentC->label) {
                    correctPredictions++;
                }

                double trueProbs[2] = {0.0};
                trueProbs[currentC->label] = 1.0;

                printf("Label = %d | digit reco = %d\n", currentC->label, digitRecognised);

                backPropagation(net, trueProbs, tP.learningRate);
            }
        }


        if(flags[0].value == 1){

            const int bar_width = 50;

            float progress = (float) (epoch_i+1) / tP.nbEpoch;
            int bar_length = progress * bar_width;

            printf("\rEpoch; %d | Progress: [", epoch_i);
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

    //freeBatch(batch);
}
