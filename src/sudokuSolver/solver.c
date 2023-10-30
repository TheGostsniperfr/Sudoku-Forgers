#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sudokuSolver/sudokuAux/sudokuUtil.h"
#include "sudokuSolver/sudokuAux/sudokuHandle.h"


Option options[] = {
    {"-p", handleConsoleGridPrint},
    {"-po", handleConsoleOutputGridPrint},
    {"-g", handleGenerateGridImg},
};


int main(int argc, char* argv[]){
    /*
        Usage :
            ./solver <inputGrid> -> solve the grid and create
                                    outputGrid.result
                                    in the same folder
    */

   char* gridPath;


    int optionFound  = 0;


    if(argc > 1 && argv[1][0] != '-'){
        gridPath = argv[1];
        handleSolver(argc, argv, gridPath);


        optionFound ++;
        argc--;
        argv++;
    }else{
        errx(EXIT_FAILURE, ERROR_NB_ARG);
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
                    argv + subArgStart, gridPath
                );

                i = subArgEnd - 1;
                break;
            }
        }

    }

    if(optionFound  == 0){
        printf("❗ Usage : ./solver <gridPath>\n");
    }



    return EXIT_SUCCESS;
}
