#include <stdio.h>


int testGrid[9][9] = {
    {5, 4, 0, 0, 2, 0, 8, 0, 6},
    {0, 1, 9, 0, 0, 7, 0, 0, 3},
    {0, 0, 0, 3, 0, 0, 2, 1, 0},
    {9, 0, 0, 4, 0, 5, 0, 2, 0},
    {0, 0, 1, 0, 0, 0, 6, 0, 4},
    {6, 0, 4, 0, 3, 2, 0, 8, 0},
    {0, 6, 0, 0, 0, 0, 1, 9, 0},
    {4, 0, 2, 0, 0, 9, 0, 0, 5},
    {0, 9, 0, 0, 7, 0, 4, 0, 2}
};


/***************************************************************
 *  Function isRulesRespected: 
 *
 *  Check if the number respects the game's rule
 *
 *  @input :
 *      - grid (int array) : the grid of the game
 *      - row (int) : row of the number to check
 *      - col (int) : col of the number to check
 *      - num (int) : number to check
 *
 *  @output :
 *      - (int) : 0 -> false, 1 -> true
***************************************************************/

int isRulesRespected(int** grid, int gS, int row, int col, int num) {

    // Check current row and column
    for (int i = 0; i < gS; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return 0; 
        }
    }

    // Check current area
    int start_row = 3 * (row / 3);
    int start_col = 3 * (col / 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[start_row + i][start_col + j] == num) {
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
 *      - grid (int array) : the grid of the game
 *      - row (int*) : pointer to store the row of the empty cell
 *      - col (int*) : pointer to store the col of the empty cell
***************************************************************/

void findEmptyCell(int** grid, int gS, int* row, int* col) {
    for (*row = 0; *row < gS; (*row)++) {
        for (*col = 0; *col < gS; (*col)++) {
            if (grid[*row][*col] == 0) {
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
 *      - source (int array) : the source grid
 *      - destination (int array) : the destination grid
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
 *      - grid (int array) : the grid to print
***************************************************************/

void printGrid(int** grid, int gS) {
    for (int i = 0; i < gS; i++) {
        for (int j = 0; j < gS; j++) {
            printf("%d ", grid[i][j]);
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
 *      - grid (int array) : the Sudoku grid to solve
 *
 *  @output :
 *      - (int) : 0 -> no solution, 1 -> solution found
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

            grid[row][col] = 0;
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
 *      - grid (int array) : the Sudoku grid to solve and print
***************************************************************/

int sudokuSolver(int** grid, int gS) {
    //int solvedGrid[9][9];
    //copyGrid(grid, solvedGrid);

    if (solver(grid, gS)) {
        //printGrid(grid);
        return 0;
    } else {
        //printf("Grid is impossible\n");
        return 1;
    }
}


/***************************************************************
 *  Function loadGrid: 
 *
 *  Open matrix file and load data into 9x9 matrix
 *
 *  @input :
 *      - filename (char) : name of the file to open
 *      - grid (int array) : grid of sudoku
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
        if (character >= '0' && character <= '9') {
            grid[row][col] = character - '0';
            col++;
            if (col == gS) {
                col = 0;
                row++;
            }
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

void saveMatrix(const char *filename, int** grid, int gS) {
    FILE *file = fopen(filename, "w"); 

    if (file == NULL) {
        perror("saveMatrix -> Error to open file");
        return; 
    }

    for (int row = 0; row < gS; row++) {
        for (int col = 0; col < gS; col++) {
            fprintf(file, "%d", grid[row][col]);
        }
        if (row != 8) {
            fprintf(file, "\n"); 
        }
    }

    fclose(file); 
}
