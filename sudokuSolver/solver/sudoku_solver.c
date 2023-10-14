#include <stdio.h>
#include <string.h>
#include <err.h>
#include "../convertLib/sudokuConvert.h"
#include <stdlib.h>


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
int isGridValid(int** grid, int gS){

    if(gS != 9 && gS != 16){
        
        return 0;
    }
    
    //create "present number sheet"
    //to check if a number is already present
    char* lNb = calloc(gS+1, sizeof(char));

    //check row lines
    for (int row = 0; row < gS; row++)
    {
        for(int col = 0; col < gS; col++){
            if(grid[row][col] == -1){
                continue;
            }

            if(lNb[grid[row][col]] == 1){
                //number already present

                return 0;
            }  
            lNb[grid[row][col]] = 1;
        }   

        //Reset present list
        memset(lNb, 0, (gS+1) * sizeof(char));
    }
    

    //check col lines
    for (int col = 0; col < gS; col++)
    {
        for(int row = 0; row < gS; row++){
            if(grid[row][col] == -1){
                continue;
            }

            if(lNb[grid[row][col]] == 1){
                //number already present

                return 0;
            }  
            lNb[grid[row][col]] = 1;
        }   

        //Reset present list
        memset(lNb, 0, (gS+1) * sizeof(char));
    }


    //check sub square
    int sbLth = gS == 9 ? 3 : 4;
    for (int i = 0; i < gS; i+= sbLth)
    {
        for (int j = 0; j < gS; j+= sbLth)
        {
            for (int x = i; x < i + sbLth; x++)
            {
                for (int y = j; y < j + sbLth; y++)
                {
                    if(grid[x][y] == -1){
                        continue;
                    }

                    if(lNb[grid[x][y]] == 1){
                        //number already present

                        return 0;
                    }  
                    lNb[grid[x][y]] = 1;
                }                
            }

            //Reset present list
            memset(lNb, 0, (gS+1) * sizeof(char));            
        }        
    }

    free(lNb);
    return 1;
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

int isRulesRespected(int** grid, int gS, int row, int col, int num) {
    //check col
    for (int i = 0; i < gS; i++)
    {
        if(grid[row][i] == num){
            return 0;
        }
    }

    //check row
    for (int i = 0; i < gS; i++)
    {
        if(grid[i][col] == num){
            return 0;
        }
    }

    //check subSquare
    int subLth = gS == 9 ? 3 : 4;
    int startRow = row - (row % subLth);
    int startCol = col - (col % subLth);
    
    for (int i = 0; i < subLth; i++)
    {
        for (int j = 0; j < subLth; j++)
        {
            if(grid[i + startRow][j + startCol] == num){
                return 0;
            }
        }        
    }
    return 1;    
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

void findEmptyCell(int** grid, int gS, int* row, int* col) {
    for (*row = 0; *row < gS; (*row)++) {
        for (*col = 0; *col < gS; (*col)++) {
            if (grid[*row][*col] == -1) {
                return;
            }
        }
    }
    *row = -1;
    *col = -1;
}


/***************************************************************
 *  Function copyGrid: 
 *
 *  Copy the values from the source grid to the destination grid
 *
 *  @input :
 *      - source (int **) : the source grid
 *      - destination (int**) : the destination grid
 *      - gS (int) : size of the grid
***************************************************************/

void copyGrid(int** source, int** destination, int gS) {
    for (int i = 0; i < gS; i++) {
        for (int j = 0; j < gS; j++) {
            destination[i][j] = source[i][j];
        }
    }
}


/***************************************************************
 *  Function printGrid: 
 *
 *  Print the Sudoku grid
 *
 *  @input :
 *      - grid (int **) : the grid to print
 *      - gS (int) : size of the grid
***************************************************************/

void printGrid(int** grid, int gS) {
    for (int i = 0; i < gS; i++) {
        for (int j = 0; j < gS; j++) {
            if(grid[i][j] == -1){
                printf(".");
            }else{                
                printf("%d", grid[i][j]);
            }            
        }
        printf("\n");
    }
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

int solver(int** grid, int gS) {
    int row, col;
    findEmptyCell(grid, gS, &row, &col);

    if (row == -1) {
        return 1;
    }

    for (int num = 1; num <= gS; num++) {
        if (isRulesRespected(grid, gS, row, col, num)) {
            grid[row][col] = num;

            if (solver(grid, gS)) {
                return 1;
            }

            grid[row][col] = -1;
        }   
    }

    return 0;
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

int sudokuSolver(int** grid, int gS) {

    if(isGridValid(grid, gS) == 0){
        printf("test18\n");
        return 0;
    }

    if (solver(grid, gS)) {
        return 1;
    } else {
        return 0;
    }
}


/***************************************************************
 *  Function loadGrid: 
 *
 *  Open matrix file and load data into 9x9 matrix
 *
 *  @input :
 *      - filename (char) : name of the file to open
 *      - grid (int**) : grid of sudoku
 *      - gS (int) : size of the grid
 * @output : 
 *      - (int) : 0 -> no error, 1 -> error to load data
***************************************************************/

int loadGrid(const char *filename, int** grid, int gS) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("loadGrid -> Error to open file");
        return 1; 
    }

    int character;
    int row = 0;
    int col = 0;

    while ((character = fgetc(file)) != EOF && row < gS && col < gS) {        

        if(character == '\n'){
            continue;
        }

        if(character == '.'){
            //-1 is the separator in the grid
            grid[row][col] = -1;
            
        }else{
            int nb = charToInt((char)character);

            //9x9 takes [1-9] possible value in grid 
            //16x16 takes [1-16] possibles value in grid
            if(gS == 16){
                nb++;
            }

            grid[row][col] = nb;
            
        }

        col++;
        if (col == gS) {
            col = 0;
            row++;
        }
        
    }

    fclose(file);

    if (row != gS || col != 0) {
        printf("File does not contain a valid matrix\n");
        return 1; 
    }

    return 0; 
}


/***************************************************************
 *  Function saveMatrix: 
 *
 *  Save grid into file
 *
 *  @input :
 *      - filename (char) : name of the file to create
 *      - grid (int array) : grid of sudoku
***************************************************************/

void saveGrid(const char *filename, int** grid, int gS) {
    FILE *file = fopen(filename, "w"); 
    if (file == NULL) {
        perror("saveMatrix -> Error to open file");
        return; 
    }

    for (int row = 0; row < gS; row++) {
        for (int col = 0; col < gS; col++) {
            if(grid[row][col] == -1){
                fprintf(file, ".");
            }
            else{
                int nb = grid[row][col];
                if(gS == 16){
                    nb--;
                }                

                fprintf(file, "%c", intToChar(nb));
            }            
        }
        
        fprintf(file, "\n"); 
        
    }

    fclose(file); 
}
