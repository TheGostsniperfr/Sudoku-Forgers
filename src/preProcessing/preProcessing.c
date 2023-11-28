#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/Gaussian_Filter/gaussianFilter.h"
#include "preProcessing/GrayScale_Filter/grayScaleFilter.h"
#include "preProcessing/Histogram_Lib/histogram.h"
#include "preProcessing/binarization_method/otsu.h"
#include "preProcessing/Morphology/morphology.h"
#include "preProcessing/binarization_method/adaptiveThreshold.h"

//#include "Rotation/rotation.h"

#include "preProcessing/preProcessingAux/preProcessingHandle.h"
#include "GUI/handleUtils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NB_FLAGS 2

OptionPrePro options[] = {
    {"-all", handleAllSteps},
    {"-r", handleTurnNDegree},
    {"--help", handlePrintPreProHelp}
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

    for (int i = 0; i < NB_FLAGS; i++)
    {
        flags->flag = NULL;
        flags->value = 0;
    }
    
    flags[0].flag = "-verbose";
    flags[1].flag = "-save";


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
    char* InputImgPath = "";

    if(argc > 1 && argv[1][0] != '-'){
        InputImgPath = argv[1];

        optionFound ++;
        argc--;
        argv++;
    }else{
        handlePrintPreProHelp(0, argv, "", flags);
        return EXIT_SUCCESS;
    }

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
                    InputImgPath,
                    flags
                );

                i = subArgEnd - 1;
                break;
            }
        }

    }

    if(optionFound == 0){
        handlePrintPreProHelp(0, argv, "", flags);
    }

    if(optionFound == 1 && InputImgPath != 0){
        //handleAllSteps(argc, argv, InputImgPath, flags);
    }



    free(flags);
    return EXIT_SUCCESS;
}