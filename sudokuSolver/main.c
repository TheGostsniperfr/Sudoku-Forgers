#include "outputGrid/createOutputGrid.h"
#include "solver/sudoku_solver.h"
#include "convertLib/sudokuConvert.h"

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

    //SDL_Surface* finalGrid = createOutputGrid(gS);
    //saveImg(finalGrid, "outputGrid.jpg");

    for (int i = 0; i < gS; i++)
    {
        free(grid[i]);        
    }
    
    free(grid);


    return 0; 
}