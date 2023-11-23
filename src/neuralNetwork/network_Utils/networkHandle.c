#include "GUI/handleUtils.h"
#include "neuralNetwork/trainingNetwork/xor/xorTraining.h"
#include "neuralNetwork/trainingNetwork/digits/digitsTraining.h"
#include "neuralNetwork/network_Utils/saveLoadNetwork.h"
#include "neuralNetwork/network_Utils/networkUtils.h"
#include "neuralNetwork/network_Utils/createNetwork.h"
#include "neuralNetwork/network_Utils/logicalBrain.h"
#include "neuralNetwork/network_Utils/mnistUtils.h"

#include "preProcessing/SDL_Function/sdlFunction.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define DEFAULT_XOR_FILENAME "xorTrain.txt"
#define DEFAULT_DIGITS_FILENAME "digitsTrain.txt"

int handleXorTrain(
        int argc,
        char* argv[],
        NeuralNetwork* net,
        Flag* flags)
    {

        //Training Parameters
        TrainingPara tP;

        if(flags[3].value == 1){
            //load default config

            tP.nbEpoch = 100;
            tP.batchSize = 1000;
            tP.learningRate = 1;
            tP.saveTraining = false;
        }else{
            if(argc != 3){
                errx(EXIT_FAILURE, ERROR_NB_ARG);
            }

            tP.nbEpoch = atoi(argv[0]);
            tP.batchSize = atoi(argv[1]);
            tP.learningRate = atof(argv[2]);
            tP.saveTraining = false;
        }

        if(flags[1].value == 1){
            tP.saveTraining = true;
        }

        if(flags[0].value == 1){
            printTrainingPara(tP);
        }


        //Neural Network Parameters

        NetworkPara netPara;

        if(net == NULL){
            if(flags[0].value == 1){
                printf
                (
                    "⚠️ No neural network specification ⚠️\n"
                    "✅ Load default neural network specification.\n"
                );
            }

            netPara.nbNeuronsFirstLayer = 2;
            netPara.nbHiddenLayers = 1;
            netPara.nbNeuronsHiddenLayer = 3;
            netPara.nbNeuronsOutputLayer = 2;

            net = createNetwork(netPara);
        }


        if(flags[0].value == 1){
            printNetworkPara(netPara);
        }

        xorTraining(net, tP, DEFAULT_XOR_FILENAME, flags);

        destroyNetwork(net);

        return EXIT_SUCCESS;
    }


int handleDigitsTrain(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        NeuralNetwork* net __attribute__((unused)),
        Flag* flags __attribute__((unused)))
    {
        //Training Parameters
        TrainingPara tP;

        if(flags[3].value == 1){
            //load default config

            tP.nbEpoch = 30;
            tP.batchSize = 20000;
            tP.learningRate = 1;
            tP.saveTraining = false;
        }else{
            if(argc != 3){
                errx(EXIT_FAILURE, ERROR_NB_ARG);
            }

            tP.nbEpoch = atoi(argv[0]);
            tP.batchSize = atoi(argv[1]);
            tP.learningRate = atof(argv[2]);
            tP.saveTraining = false;
        }

        if(flags[1].value == 1){
            tP.saveTraining = true;
        }

        if(flags[0].value == 1){
            printTrainingPara(tP);
        }

        NetworkPara netPara;

        if(net == NULL){
            if(flags[0].value == 1){
                printf
                (
                    "⚠️ No neural network specification ⚠️\n"
                    "✅ Load default neural network specification.\n"
                );
            }

            netPara.nbNeuronsFirstLayer = 784;
            netPara.nbHiddenLayers = 1;
            netPara.nbNeuronsHiddenLayer = 80;
            netPara.nbNeuronsOutputLayer = 10;

            net = createNetwork(netPara);
        }


        if(flags[0].value == 1){
            printNetworkPara(netPara);
        }

        digitTraining(net, tP, DEFAULT_DIGITS_FILENAME, flags);

        destroyNetwork(net);

        return EXIT_SUCCESS;
    }


int handleLoad(
        int argc,
        char* argv[],
        NeuralNetwork* net __attribute__((unused)),
        Flag* flags)
    {
        if(argc != 1){
            errx(EXIT_FAILURE, ERROR_NB_ARG);
        }

        if(flags[0].value == 1){
            printf("🚀 Starting to load neural network.\n");
        }

        net = loadNetwork(argv[0]);

        if(flags[0].value == 1){
            printf("✅ Success to load neural network.\n");
        }

        return EXIT_SUCCESS;
    }


int handleTestXor(
        int argc ,
        char* argv[] ,
        NeuralNetwork* net ,
        Flag* flags )
    {

        if(argc != 3){
            errx(EXIT_FAILURE, ERROR_NB_ARG);
        }

        if(flags[0].value == 1){
            printf("🚀 Starting to load neural network.\n");
        }

        net = loadNetwork(argv[0]);

        if(flags[0].value == 1){
            printf("✅ Success to load neural network.\n");


            printNetworkSpec(net);
        }

        double inputA = atof(argv[1]);
        double inputB = atof(argv[2]);
        int expected = inputA != inputB;

        double input[2] = {inputA, inputB};


        double output[2];

        forwardPropagation(net, input);

        Layer* lL = &net->layers[net->nb_layers-1];
        output[0] = lL->neurons[0].output;
        output[1] = lL->neurons[1].output;

        int digitRecognised = 0;
        for (int i = 1; i < net->layers[0].nb_neurons; i++) {
            if (output[i] > output[digitRecognised]) {
                digitRecognised = i;
            }
        }

        printf
        (
            "------------------------------------\n"
            "                 |\n"
            "  🅰️  Input 1 : %d |  🅱️  Input 2 : %d\n"
            "                 |\n"
            "------------------------------------\n\n"

            "✅ Expected value : %d\n"
            "⚡ value found : %d\n",

            (int)inputA,
            (int)inputB,
            expected,
            digitRecognised
        );


        destroyNetwork(net);

        return EXIT_SUCCESS;
    }

int handleTestDigit(
        int argc ,
        char* argv[] ,
        NeuralNetwork* net ,
        Flag* flags )
    {

        if(argc != 2){
            errx(EXIT_FAILURE, ERROR_NB_ARG);
        }

        if(flags[0].value == 1){
            printf("🚀 Starting to load neural network.\n");
        }

        net = loadNetwork(argv[0]);

        if(flags[0].value == 1){
            printf("✅ Success to load neural network.\n");
            printNetworkSpec(net);
        }

        int imgIndex = atoi(argv[1]);

        ImgContainer* imgContainer = getImageFromMnist(imgIndex, flags);

        if(flags[1].value == 1){
            saveImg(imgContainer->img, "associateImage.jpg");
            if(flags[0].value == 1){
                printf("💾 Success to save %dth image.\n", imgIndex);
            }
        }
        
        double input[784];

        for (int px_i = 0; px_i < 784; px_i++)
        {
            Uint32* pixels = imgContainer->img->pixels;

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
        
        printf
        (
            "------------------------------------\n\n"

            "        📷 Image index : %d\n\n"
            
            "------------------------------------\n\n"

            "✅ Expected value : %d\n"
            "⚡ value found : %d\n",

            imgIndex,
            imgContainer->label,
            digitRecognised
        );


        destroyNetwork(net);

        return EXIT_SUCCESS;
    }


int handlePrintHelp(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        NeuralNetwork* net __attribute__((unused)),
        Flag* flags __attribute__((unused)))
    {
    printf(
            "Usage : network [OPTIONS]\n\n"
            "-xorTrain  <number epoch> <batch size> <learning rate>\n"
            "                         ->  Start xor training\n"
            "-digitsTrain <number epoch> <batch size> <learning rate>\n"
            "                         ->  Start digits training\n"
            "-testXor <dir> <index>   ->      Test xor network\n"
            "-testDigit <dir> <index> ->      Test digit network\n"
            "-verbose                 ->      Print informations\n"
            "-save                    ->      Save neural network\n"
            "-defaultNetSpec          ->      Load default network specification\n"
            "-defaultTrainSpec        ->      Load default training specification\n"
            "-showImg <index>         ->      Show the image at the n index\n"
            "--help                   ->      Show the help panel\n"
        );

        return EXIT_SUCCESS;
    }


int handleGetImgFromMnist(
    int argc,
    char* argv[],
    NeuralNetwork* net __attribute__((unused)),
    Flag* flags)
    {
        if(argc != 1){
            errx(EXIT_FAILURE, ERROR_NB_ARG);
        }

        ImgContainer* imgContainer = getImageFromMnist(atoi(argv[0]), flags);



        if(flags[1].value == 1) {
            printf("Image label : %d\n", imgContainer->label);
            saveImg(imgContainer->img, "OutDigit.jpg");
            if(flags[0].value == 1){
                printf("💾 Success to save OutDigit.jpg\n");
            }
        }


        freeImgContainer(imgContainer);

        return EXIT_SUCCESS;
    }