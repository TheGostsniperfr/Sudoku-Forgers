#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

typedef struct SudokuGrid
{
    int** grid;
    int gS;
} SudokuGrid;


/***************************************************************
 *  Function charToInt:
 *
 *  A simple function to convert char hexa character
 *  to int representation
 *
 *  @input :
 *      - str (char) : character to convert
 *
 *  @output :
 *      - (int) : int hexa representation
***************************************************************/
int charToInt(char str){
    if(str >= '0' && str <= '9'){
        return str - '0';
    }

    if(str >= 'A' && str <= 'F'){
        return str - 'A' + 10;
    }

    if(str >= 'a' && str <= 'f'){
        return str - 'a' + 10;
    }

    return -1;
}


/***************************************************************
 *  Function intToChar:
 *
 *  A simple function to convert int to hexa character
 *  to int representation
 *
 *  @input :
 *      - nb (int) : number to convert
 *
 *  @output :
 *      - (char) : Character hexa representation
***************************************************************/
char intToChar(int nb){
    if(nb >= 0 && nb <= 9){
        return ('0' + nb);
    }

    if(nb >= 10 && nb <= 15){
        return ('A' + nb - 10);
    }

    return -1;
}


/***************************************************************
 *  Function gridSize:
 *
 *  Return the size of the grid, either 9 or 16 (Hexa grid)
 *
 *  @input :
 *      - str (char) : character to convert
 *
 *  @output :
 *      - (int) : Character hexa representation
***************************************************************/
int gridSize(const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        errx(1, "Error : gridSize can't open %s.\n", filename);
        return -1;
    }

    int count = 0;
    int character;

    while ((character = fgetc(file)) != EOF && character != '\n') {
        count++;
    }

    fclose(file);

    if(count != 9 && count != 16){
        printf("Value count %d\n", count);
        errx(1, "Error : Invalid grid format.");
    }else{
        return count;
    }
}


/***************************************************************
 *  Function allocGrid:
 *
 *  Alloc the memory space for a grid size give
 *
 *  @input :
 *      - gs (int) : size of the grid
 *
 *  @output :
 *      - grid (int**) : empty grid
***************************************************************/
int** allocGrid(int gS){
    int** grid = (int**)calloc(gS, sizeof(int*));
    for(int i = 0; i < gS; i++){
        grid[i] = (int*)calloc(gS, sizeof(int));
    }

    return grid;
}

/***************************************************************
 *  Function freeGrid:
 *
 *  Free a grid inside a SudokuGrid obj
 *
 *  @input :
 *      - grid (int**) : grid to free
 *      - gs (int) : size of the grid
***************************************************************/
void freeGrid(SudokuGrid sG){
    for (int i = 0; i < sG.gS; i++)
    {
        free(sG.grid[i]);
    }

    free(sG.grid);
}


/***************************************************************
 *  Function saveGrid:
 *
 *  Save grid into file
 *
 *  @input :
 *      - filename (char) : name of the file to create
 *      - grid (int array) : grid of sudoku
***************************************************************/
int saveGrid(const char *filename, SudokuGrid sG) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        errx(EXIT_FAILURE , "saveMatrix -> Error to open file");
    }

    for (int row = 0; row < sG.gS; row++) {
        for (int col = 0; col < sG.gS; col++) {
            if(sG.grid[row][col] == -1){
                fprintf(file, ".");
            }
            else{
                int nb = sG.grid[row][col];
                if(sG.gS == 16){
                    nb--;
                }

                fprintf(file, "%c", intToChar(nb));
            }
        }

        fprintf(file, "\n");

    }

    fclose(file);
    return EXIT_SUCCESS;
}


/***************************************************************
 *  Function loadGrid:
 *
 *  Open matrix file and load data into 9x9 matrix
 *
 *  @input :
 *      - filename (char) : name of the file to open
 * @output :
 *      - grid (int**) : the grid loaded
***************************************************************/
SudokuGrid loadGrid(const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        errx(EXIT_FAILURE, "loadGrid -> Error to open file");
    }

    int size = gridSize(filename);
    SudokuGrid sG = {
        .gS = size,
        .grid = allocGrid(size)
    };

    int character;
    int row = 0;
    int col = 0;

    while ((character = fgetc(file)) != EOF && row < sG.gS && col < sG.gS) {

        if(character == '\n'){
            continue;
        }

        if(character == '.'){
            //-1 is the separator in the grid
            sG.grid[row][col] = -1;

        }else{
            int nb = charToInt((char)character);

            //9x9 takes [1-9] possible value in grid
            //16x16 takes [1-16] possibles value in grid
            if(sG.gS == 16){
                nb++;
            }

            sG.grid[row][col] = nb;

        }

        col++;
        if (col == sG.gS) {
            col = 0;
            row++;
        }

    }

    fclose(file);

    if (row != sG.gS || col != 0) {
        errx(EXIT_FAILURE, "File does not contain a valid matrix");
    }

    return sG;
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
void printGrid(SudokuGrid sG) {
    for (int i = 0; i < sG.gS; i++) {
        for (int j = 0; j < sG.gS; j++) {
            if(sG.grid[i][j] == -1){
                printf(".");
            }else{
                printf("%d", sG.grid[i][j]);
            }
        }
        printf("\n");
    }
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
void copyGrid(SudokuGrid sG1, SudokuGrid sG2) {
    for (int i = 0; i < sG1.gS; i++) {
        for (int j = 0; j < sG1.gS; j++) {
            sG2.grid[i][j] = sG1.grid[i][j];
        }
    }
}



char* concateStr(char* a1, char* a2){
    char* result = malloc(strlen(a1) + strlen(a2) + 1);

    if(result == NULL){
        errx(EXIT_FAILURE, "Failled to allocated memory for concate result.");
    }

    strcpy(result, a1);
    strcat(result, a2);

    return result;
}
