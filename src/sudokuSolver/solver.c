#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "sudokuSolver/sudokuAux/sudokuUtil.h"
#include "sudokuSolver/sudokuAux/sudokuHandle.h"
#include "GUI/handleUtils.h"

#define NB_FLAGS 4


Option options[] = {
    {"-g", handleGenerateGridImg},
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
    flags[1].flag = "-p";
    flags[2].flag = "-po";
    flags[3].flag = "-g";


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

    char* gridPath;
    int optionFound  = 0;


    if(argc > 1 && argv[1][0] != '-'){
        gridPath = argv[1];
        handleSolver(argc, argv, gridPath, flags);
        optionFound ++;
        argc--;
        argv++;
    }else{
        handlePrintHelp(0, argv, "", flags);
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
                    gridPath,
                    flags
                );

                i = subArgEnd - 1;
                break;
            }
        }

    }

    if(optionFound == 0){
        handlePrintHelp(0, argv, "", flags);
    }



    return EXIT_SUCCESS;
}
