#include "outputGrid/createOutputGrid.h"
#include "solver/sudoku_solver.h"

int main(){
    const char *inputFileName = "../data/input_matrix/input_matrix.result";
    const char *outputFileName = "grid.result";

    int testGrid[9][9];

    if (loadGrid(inputFileName, testGrid) == 0) {

        //solve grid : 
        sudokuSolver(testGrid);

        saveMatrix(outputFileName, testGrid);

        printf("The matrix was successfully saved : %s\n", outputFileName);
    } else {
        printf("An error has occurred while loading the file : %s\n", inputFileName);
        return 1; 
    }

    createOutputGrid(outputFileName);


    return 0; 
}