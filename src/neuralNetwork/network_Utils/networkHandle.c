#include "GUI/handleUtils.h"
#include "neuralNetwork/trainingNetwork/xor/xorTraining.h"
#include "neuralNetwork/trainingNetwork/digits/digitsTraining.h"
#include "neuralNetwork/network_Utils/saveLoadNetwork.h"
#include "neuralNetwork/network_Utils/networkUtils.h"
#include "neuralNetwork/network_Utils/createNetwork.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include <stdbool.h>

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
            tP.batchSize = 4;
            tP.learningRate = 0.01;
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
            netPara.nbNeuronsHiddenLayer = 5;
            netPara.nbNeuronsOutputLayer = 1;

            net = createNetwork(netPara);
        }


        if(flags[0].value == 1){
            printNetworkPara(netPara);
        }

        xorTraining(net, tP, DEFAULT_XOR_FILENAME, flags);

        return EXIT_SUCCESS;
    }


int handleDigitsTrain(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        NeuralNetwork* net __attribute__((unused)),
        Flag* flags __attribute__((unused)))
    {
        //TODO

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


int handleTest(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        NeuralNetwork* net __attribute__((unused)),
        Flag* flags __attribute__((unused)))
    {
        //TODO

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
            "                   ->  Start xor training\n"
            "-digitsTrain <number epoch> <batch size> <learning rate>\n"
            "                   ->  Start digits training\n"
            "-load <path>       ->      Load a neural network\n"
            "-test <input>      ->      Test network with input\n"
            "-verbose           ->      Print informations\n"
            "-save              ->      Save neural network\n"
            "-defaultNetSpec    ->      Load default network specification\n"
            "-defaultTrainSpec  ->      Load default training specification\n"
            "--help             ->      Show the help panel\n"
        );

        return EXIT_SUCCESS;
    }