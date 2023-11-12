
#include "GUI/handleUtils.h"
#include "neuralNetwork/network_Utils/networkHandle.h"
#include "neuralNetwork/network_Utils/createNetwork.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NB_FLAGS 4

Option options[] = {
    {"-xorTrain", handleXorTrain},
    {"-digitsTrain", handleDigitsTrain},
    {"-load", handleLoad},
    {"-test", handleTest},
    {"--help", handlePrintHelp},
    {"-showImg", handleGetImgFromMnist}
};





int main(int argc, char* argv[]){
    /*
        Usage :
            ./solver <inputGrid> -> solve the grid and create
                                    outputGrid.result
                                    in the same folder
    */


   

    //Init flags
    Flag* flags = (Flag*)malloc(NB_FLAGS * sizeof(Flag));
    NeuralNetwork* net = NULL;

    for (int i = 0; i < NB_FLAGS; i++)
    {
        flags->flag = NULL;
        flags->value = 0;
    }

    flags[0].flag = "-verbose";
    flags[1].flag = "-save";
    flags[2].flag = "-defaultNetSpec";
    flags[3].flag = "-defaultTrainSpec";


    //find flags

    for (int i = 1; i < argc; i++)
    {
        for (size_t j = 0; j < NB_FLAGS; j++)
        {
            if(strcmp(argv[i], flags[j].flag) == 0){
                flags[j].value = 1;
            }

        }
    }

    int optionFound  = 0;

    for (int i = 1; i < argc; i++)
    {
        char* currentArg = argv[i];

        for (size_t j = 0; j < sizeof(options) / sizeof(options[0]); j++)
        {

            if(strcmp(currentArg, options[j].flag) == 0){
                optionFound = 1;
                int subArgStart = i + 1;
                int subArgEnd = subArgStart;
                while(subArgEnd < argc && argv[subArgEnd][0] != '-'){
                    subArgEnd++;
                }

                options[j].action(
                    subArgEnd - subArgStart,
                    argv + subArgStart,
                    net,
                    flags
                );

                i = subArgEnd - 1;
                break;
            }
        }

    }

    if(optionFound == 0){
        handlePrintHelp(0, argv, net, flags);
    }


    free(flags);

    return EXIT_SUCCESS;
}