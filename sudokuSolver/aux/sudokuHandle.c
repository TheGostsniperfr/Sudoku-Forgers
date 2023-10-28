#include "sudokuUtil.h"
#include"../sudokuSolver/sudoku_solver.h"
#include <string.h>
#include "sudokuUtil.h"
#include "../outputGrid/createOutputGrid.h"
#include "sudokuUtil.h"

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


int handleConsoleGridPrint(int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* gridPath){
    /*
        Usage :
            [-p|-print] -> print the path grid
    */


    SudokuGrid sG = loadGrid(gridPath);

    printGrid(sG);

    freeGrid(sG);

    return EXIT_SUCCESS;
}


int handleGenerateGridImg(int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* gridPath){
    /*
        Usage :
            [-g|-generateImg] -> generate an output grid
    */

    SudokuGrid defaultSG = loadGrid(gridPath);

    SudokuGrid solvedSG = loadGrid(concateStr(gridPath, ".result"));

    SDL_Surface* outImg = createOutputGrid(defaultSG, solvedSG);

    saveImg(outImg, concateStr(gridPath, ".jpg"));

    freeGrid(defaultSG);
    freeGrid(solvedSG);

    return EXIT_SUCCESS;
}
