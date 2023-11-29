#include <string.h>
#include <libgen.h>

#include "sudokuSolver/sudokuSolver/sudoku_solver.h"
#include "sudokuSolver/sudokuAux/sudokuUtil.h"
#include "sudokuSolver/outputGrid/createOutputGrid.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

#include "GUI/handleUtils.h"



/***************************************************************
 *  Function handleSolver:
 *
 *  handle function to solve the input grid
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - gridPath (char*) : path of the input grid
 *      - flags (Flag*) : flag of the program
 *
 * @output :
 *      - (int) : state of the program
***************************************************************/

void* handleSolver(int argc,
        char* argv[] __attribute__((unused)),
        char* gridPath,
        Flag* flags)
    {

    /*
        Usage:
            [⚠️First argument⚠️] <grid path> -> solved the path grid
    */

    if(argc < 1){
        errx(EXIT_FAILURE, ERROR_NB_ARG);
    }

    if(flags[0].value == 1){
        printf("🚀 Starting to solve\n");
    }

    SudokuGrid sG = loadGrid(gridPath);

    if(flags[2].value == 1){
        printSection(gridPath);
        printGrid(sG);
    }

    //solve grid :
    if(sudokuSolver(sG) == 0){
        errx(EXIT_FAILURE, "Grid is impossible.");
    }

    char path[1024];

    char* fileName = strrchr(gridPath, '/');
    if(fileName == NULL){
        snprintf(path, 1024, "%s/%s.result",getenv("CURRENT_DIR"), gridPath);
    }else{
        snprintf(path, 1024, "%s%s.result",getenv("CURRENT_DIR"), fileName);
    }


    printf("Save path = %s\n", path);
    saveGrid(path, sG);

    if(flags[3].value == 1){
        printSection(concateStr(gridPath, ".result"));

        printGrid(sG);
    }

    freeGrid(sG);

    if(flags[0].value == 1){
        printf("✅ Success to solve grid\n");
    }

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
 *      - flags (Flag*) : flag of the program
 *
 *  @output :
*       (int) : state of the program
***************************************************************/

void* handleGenerateGridImg(int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* gridPath,
        Flag* flags)
    {
    /*
        Usage :
            [-g|-generateImg] -> generate an output grid
    */

   if(argc == 0){
        errx(EXIT_FAILURE, "Usage : -g <grid width (px)> "
                            "<border ratio> <width border> <font ratio>");
   }

   if(argc != 4){
        errx(EXIT_FAILURE, ERROR_NB_ARG);
   }

    if(flags[0].value == 1){
        printf("🚀 Starting to generate image.\n");
    }

    SudokuGrid defaultSG = loadGrid(gridPath);

    char path[1024];

    char* fileName = strrchr(gridPath, '/');
    if(fileName == NULL){
        snprintf(path, 1024, "%s/%s.result", getenv("CURRENT_DIR"), gridPath);
    }else{
        snprintf(path, 1024, "%s%s.result", getenv("CURRENT_DIR"), fileName);
    }

    SudokuGrid solvedSG = loadGrid(path);

    GridPara gP = {
        .gridPxSize = atof(argv[0]),
        .borderRatio = atof(argv[1]),
        .widthBorder = atof(argv[2]),
        .fontRatio = atof(argv[3])
    };

    SDL_Surface* outImg = createOutputGrid(defaultSG, solvedSG, gP);

    if(fileName == NULL){
        snprintf(path, 1024, "%s/%s.jpg", getenv("CURRENT_DIR"), gridPath);
    }else{
        snprintf(path, 1024, "%s%s.jpg", getenv("CURRENT_DIR"), fileName);
    }


    saveImg(outImg, path);

    freeGrid(defaultSG);
    freeGrid(solvedSG);

    if(flags[0].value == 1){
        printf("✅ Success to generate image.\n");
    }

    return EXIT_SUCCESS;
}


/***************************************************************
 *  Function handlePrintHelp :
 *
 *  Handle function to print help menu
 *
 *  @input :
 *      - argc (int) : number of args
 *      - argv[] (char*) : list of arg
 *      - gridPath (char*) : path of the input grid
 *      - flags (Flag*) : flag of the program
 *
 *  @output :
 *      - (int) : state of the program
***************************************************************/

void* handlePrintSolverHelp(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* inputImgPath __attribute__((unused)),
        Flag* flags __attribute__((unused)))
    {
    /*
        Usage :
            [--help] -> print the help
    */

   printf(
        "Usage : solver [Grid DIR] [OPTIONS]\n\n"
        "-p             ->      Print input grid\n"
        "-po            ->      Print output grid\n"
        "-g <grid width (px)> <border ratio> <width border> <font ratio>\n"
        "               ->      Generate output image\n"
        "-verbose       ->      Print verbose\n"
        "--help         ->      Show the help panel\n"
    );

    return EXIT_SUCCESS;
}
