#include "outputGrid/createOutputGrid.h"
#include "solver/sudoku_solver.h"

int main(){
    const char *inputFileName = "../data/input_matrix/input_matrix.result";
    const char *outputFileName = "grid.result";

    int gridSize = 9;

    int** grid = (int**)calloc(gridSize, sizeof(int*));
    for(int i = 0; i < gridSize; i++){
        grid[i] = (int*)calloc(gridSize, sizeof(int));
    }

    if (loadGrid(inputFileName, grid, gridSize) == 0) {

        //solve grid : 
        if(sudokuSolver(grid, gridSize) == 1){
            //impossible grid;
            
        }

        saveMatrix(outputFileName, grid, gridSize);

        printf("The matrix was successfully saved : %s\n", outputFileName);
    } else {
        printf("An error has occurred while loading the file : %s\n", inputFileName);
        return 1; 
    }

    SDL_Surface* finalGrid = createOutputGrid(gridSize);
    saveImg(finalGrid, "outputGrid.jpg");
    free(grid);


    return 0; 
}