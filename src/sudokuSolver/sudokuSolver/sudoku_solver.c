#include <stdio.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>

#include "sudokuSolver/sudokuAux/sudokuUtil.h"



/***************************************************************
 *  Function isGridValid:
 *
 *  Check if the sudoku grid is valid
 *
 *  @input :
 *      - grid (int **) : the grid of the game
 *      - gS (int) : size of the grid
 *
 *  @output :
 *      - (int) : 0 -> false, 1 -> true
***************************************************************/
int isGridValid(SudokuGrid sG){

    if(sG.gS != 9 && sG.gS != 16){
        errx(EXIT_FAILURE, "⚠️ Dimention of the sudoku grid is invalid. ⚠️");
    }

    //create "present number sheet"
    //to check if a number is already present
    char* lNb = calloc(sG.gS+1, sizeof(char));

    //check row lines
    for (int row = 0; row < sG.gS; row++)
    {
        for(int col = 0; col < sG.gS; col++){
            if(sG.grid[row][col] == -1){
                continue;
            }

            if(lNb[sG.grid[row][col]] == 1){
                //number already present

                return EXIT_FAILURE;
            }
            lNb[sG.grid[row][col]] = 1;
        }

        //Reset present list
        memset(lNb, 0, (sG.gS+1) * sizeof(char));
    }


    //check col lines
    for (int col = 0; col < sG.gS; col++)
    {
        for(int row = 0; row < sG.gS; row++){
            if(sG.grid[row][col] == -1){
                continue;
            }

            if(lNb[sG.grid[row][col]] == 1){
                //number already present

                return EXIT_FAILURE;
            }
            lNb[sG.grid[row][col]] = 1;
        }

        //Reset present list
        memset(lNb, 0, (sG.gS+1) * sizeof(char));
    }


    //check sub square
    int sbLth = sG.gS == 9 ? 3 : 4;
    for (int i = 0; i < sG.gS; i+= sbLth)
    {
        for (int j = 0; j < sG.gS; j+= sbLth)
        {
            for (int x = i; x < i + sbLth; x++)
            {
                for (int y = j; y < j + sbLth; y++)
                {
                    if(sG.grid[x][y] == -1){
                        continue;
                    }

                    if(lNb[sG.grid[x][y]] == 1){
                        //number already present

                        return EXIT_FAILURE;
                    }
                    lNb[sG.grid[x][y]] = 1;
                }
            }

            //Reset present list
            memset(lNb, 0, (sG.gS+1) * sizeof(char));
        }
    }

    free(lNb);
    return EXIT_SUCCESS;
}



/***************************************************************
 *  Function isRulesRespected:
 *
 *  Check if the number respects the game's rule
 *
 *  @input :
 *      - grid (int **) : the grid of the game
 *      - gS (int) : size of the grid
 *      - row (int) : row of the number to check
 *      - col (int) : col of the number to check
 *      - num (int) : number to check
 *
 *  @output :
 *      - (int) : 0 -> false, 1 -> true
***************************************************************/

int isRulesRespected(SudokuGrid sG, int row, int col, int num) {
    //check col
    for (int i = 0; i < sG.gS; i++)
    {
        if(sG.grid[row][i] == num){
            return EXIT_FAILURE;
        }
    }

    //check row
    for (int i = 0; i < sG.gS; i++)
    {
        if(sG.grid[i][col] == num){
            return EXIT_FAILURE;
        }
    }

    //check subSquare
    int subLth = sG.gS == 9 ? 3 : 4;
    int startRow = row - (row % subLth);
    int startCol = col - (col % subLth);

    for (int i = 0; i < subLth; i++)
    {
        for (int j = 0; j < subLth; j++)
        {
            if(sG.grid[i + startRow][j + startCol] == num){
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}


/***************************************************************
 *  Function findEmptyCell:
 *
 *  Find the next empty cell in the grid
 *
 *  @input :
 *      - grid (int **) : the grid of the game
 *      - gS (int) : size of the grid
 *      - row (int*) : pointer to store the row of the empty cell
 *      - col (int*) : pointer to store the col of the empty cell
***************************************************************/

void findEmptyCell(SudokuGrid sG, int* row, int* col) {
    for (*row = 0; *row < sG.gS; (*row)++) {
        for (*col = 0; *col < sG.gS; (*col)++) {
            if (sG.grid[*row][*col] == -1) {
                return;
            }
        }
    }
    *row = -1;
    *col = -1;
}

/***************************************************************
 *  Function solver:
 *
 *  Solve the Sudoku puzzle recursively
 *
 *  @input :
 *      - grid (int **) : the Sudoku grid to solve
 *
 *  @output :
 *      - (int) : 0 -> no solution, 1 -> solution found
 *      - gS (int) : size of the grid
***************************************************************/

int solver(SudokuGrid sG) {
    int row, col;
    findEmptyCell(sG, &row, &col);

    if (row == -1) {
        return EXIT_FAILURE;
    }

    for (int num = 1; num <= sG.gS; num++) {
        if (isRulesRespected(sG, row, col, num) == EXIT_SUCCESS) {
            sG.grid[row][col] = num;

            if (solver(sG) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }

            sG.grid[row][col] = -1;
        }
    }

    return EXIT_SUCCESS;
}


/***************************************************************
 *  Function sudokuSolver:
 *
 *  Solve the Sudoku puzzle and print the solution
 *
 *  @input :
 *      - grid (int **) : the Sudoku grid to solve and print
 *      - gS (int) : size of the grid
 *
 *  @output :
 *      - (bool) : sucess -> 1, else 0
***************************************************************/

int sudokuSolver(SudokuGrid sG) {

    if(isGridValid(sG) == EXIT_FAILURE){
        printf("⚠️ Invalid grid. ⚠️\n");
        return EXIT_FAILURE;
    }

    return solver(sG);
}



