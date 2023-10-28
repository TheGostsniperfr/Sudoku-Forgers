#include "aux/sudokuUtil.h"
#include "aux/sudokuHandle.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Option options[] = {
    {"-p", handleConsoleGridPrint},
    {"-print", handleConsoleGridPrint},
    {"-g", handleGenerateGridImg},
    {"-generateImg", handleGenerateGridImg},
};


int main(int argc, char* argv[]){
    /*
        Usage :
            ./solver <inputGrid> -> solve the grid and create outputGrid.result
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



                options[j].action(subArgEnd - subArgStart, argv + subArgStart, gridPath);

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


/*
int main(){
    const char *inputFileName = "../data/input_matrix/input_matrix16x162";
    const char *outputFileName = "grid.result";

    int gS = gridSize(inputFileName);

    int** grid = (int**)calloc(gS, sizeof(int*));
    for(int i = 0; i < gS; i++){
        grid[i] = (int*)calloc(gS, sizeof(int));
    }


    if (loadGrid(inputFileName, grid, gS) == 0) {


        //solve grid :
        if(sudokuSolver(grid, gS) == 0){
            //impossible grid;
            printf("grid is impossible\n");

        }

        saveGrid(outputFileName, grid, gS);


        printf("The matrix was successfully saved : %s\n", outputFileName);
    } else {
        printf("An error has occurred while loading the file : %s\n", inputFileName);
        return 1;
    }



    SDL_Surface* finalGrid = createOutputGrid(gS);
    saveImg(finalGrid, "outputGrid.jpg");

    for (int i = 0; i < gS; i++)
    {
        free(grid[i]);
    }

    free(grid);


    return 0;
}*/