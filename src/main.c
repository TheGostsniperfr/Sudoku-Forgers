#include "main.h"

#define NB_FLAGS 5
#define GRID_DIM 9
#define DEFAULT_GRID_DIR "sudoku_grid"


OptionNet optionsNet[] = {
    {"-xorTrain", handleXorTrain},
    {"-digitsTrain", handleDigitsTrain},
    {"-testXor", handleTestXor},
    {"-testDigit", handleTestDigit},
    {"--help", handlePrintNetHelp},
    {"-getImg", handleGetImgFromMnist},
};

OptionSolver optionsSolver[] = {
    {"-g", handleGenerateGridImg},
    {"--help", handlePrintSolverHelp},
};

void printHelpPanel(){
    printf
    (
        "Usage : main [OPTIONS]\n\n"
        "-ocr [dir] [option]        ->      solve the image grid\n"
        "-solver [dir] [option]     ->      access to solver app\n"
        "-nn [option]               ->      access to neural network app\n"
        "-save                      ->      Save intermediate step\n"
        "-verbose                   ->      Print verbose\n"
        "--help                     ->      Show the help panel\n"
    );
}


int main(int argc, char* argv[]){
    /*
            Usage : main [OPTIONS]

            -save         ->      Save intermediate step
            -verbose      ->      Print verbose
            --help        ->      Show the help panel
    */

    if(argc <= 1){
        printHelpPanel();
        return EXIT_SUCCESS;
    }


    //Init flags
    Flag* flags = (Flag*)malloc(NB_FLAGS * sizeof(Flag));

    for (int i = 0; i < NB_FLAGS; i++)
    {
        flags->flag = NULL;
        flags->value = 0;
    }

    flags[0].flag = "-verbose";
    flags[1].flag = "-save";
    flags[2].flag = "-p";
    flags[3].flag = "-po";
    flags[4].flag = "-g";


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

    if(strcmp(argv[1], "-ocr") == 0){
        if(argc < 3){
            printHelpPanel();
            return EXIT_SUCCESS;
        }

        GridCell* gC = (GridCell*)handleAllSteps(argc, argv+2, argv[2], flags);
        gC = findAllDigits(gC, GRID_DIM*GRID_DIM, flags);

        SudokuGrid sG = gridCellToSudokuGrid(gC, GRID_DIM);

        if(flags[2].value == 1){
            printSection(DEFAULT_GRID_DIR);

            printGrid(sG);
        }

        //solve grid :
        if(sudokuSolver(sG) == 0){
            errx(EXIT_FAILURE, "Grid is impossible.");
        }


        saveGrid(concateStr(DEFAULT_GRID_DIR, ".result"), sG);

        if(flags[3].value == 1){
            printSection(concateStr(DEFAULT_GRID_DIR, ".result"));

            printGrid(sG);
        }

        freeGrid(sG);

        if(flags[0].value == 1){
            printf("✅ Success to solve grid\n");
        }
        return EXIT_SUCCESS;

    } else if (strcmp(argv[1], "-solver") == 0){
        //TODO
        return EXIT_SUCCESS;
    } else if(strcmp(argv[1], "-nn") == 0){
        //TODO
        return EXIT_SUCCESS;
    }

    //Show main help menu
    printHelpPanel();

    free(flags);
    return EXIT_SUCCESS;
}