#include "sudokuUtil.h"
#include"../sudokuSolver/sudoku_solver.h"
#include <string.h>
#include "sudokuUtil.h"
#include "../outputGrid/createOutputGrid.h"
#include "sudokuUtil.h"



/***************************************************************
 *  Function handleSolver:
 *
 *  handle function to solve the input grid
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - gridPath (char*) : path of the input grid
***************************************************************/

int handleSolver(int argc,
        char* argv[] __attribute__((unused)),
        char* gridPath){

    /*
        Usage:
            [⚠️First argument⚠️] <grid path> -> solved the path grid
    */

    if(argc < 1){
        errx(EXIT_FAILURE, ERROR_NB_ARG);
    }

    SudokuGrid sG = loadGrid(gridPath);

    //solve grid :
    if(sudokuSolver(sG) == 0){
        errx(EXIT_FAILURE, "Grid is impossible.");
    }


    saveGrid(concateStr(gridPath, ".result"), sG);

    freeGrid(sG);

    return EXIT_SUCCESS;
}


/***************************************************************
 *  Function handleConsoleGridPrint:
 *
 *  handle function to print the gridPath grid
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - gridPath (char*) : path of the input grid
***************************************************************/

int handleConsoleGridPrint(int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* gridPath){
    /*
        Usage :
            [-p] -> print the input grid
    */


    SudokuGrid sG = loadGrid(gridPath);

    char* splits = strrchr(gridPath, '/');
    char* fileName = gridPath;

    if(splits != NULL){
        fileName = splits + 1;
    }

    printSection(fileName);

    printGrid(sG);

    freeGrid(sG);

    return EXIT_SUCCESS;
}


/***************************************************************
 *  Function handleConsoleOutputGridPrint:
 *
 *  handle function to print the output grid
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - gridPath (char*) : path of the input grid
***************************************************************/

int handleConsoleOutputGridPrint(int argc, char* argv[] ,char* gridPath){

    /*
        Usage :
            [-po] -> print output grid
    */

    handleConsoleGridPrint(argc, argv, concateStr(gridPath, ".result"));

    return EXIT_SUCCESS;
}


/***************************************************************
 *  Function handleGenerateGridImg:
 *
 *  handle function to generate the output grid image
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - gridPath (char*) : path of the input grid
***************************************************************/

int handleGenerateGridImg(int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* gridPath){
    /*
        Usage :
            [-g|-generateImg] -> generate an output grid
    */

   if(argc == 0){
        errx(EXIT_FAILURE, "Usage : -g <grid width (px)>"
                            "<border ratio> <width border> <font ratio>");
   }

   if(argc != 4){
        errx(EXIT_FAILURE, ERROR_NB_ARG);
   }

    SudokuGrid defaultSG = loadGrid(gridPath);

    SudokuGrid solvedSG = loadGrid(concateStr(gridPath, ".result"));

    GridPara gP = {
        .gridPxSize = atof(argv[0]),
        .borderRatio = atof(argv[1]),
        .widthBorder = atof(argv[2]),
        .fontRatio = atof(argv[3])
    };

    SDL_Surface* outImg = createOutputGrid(defaultSG, solvedSG, gP);

    saveImg(outImg, concateStr(gridPath, ".jpg"));

    freeGrid(defaultSG);
    freeGrid(solvedSG);

    return EXIT_SUCCESS;
}
