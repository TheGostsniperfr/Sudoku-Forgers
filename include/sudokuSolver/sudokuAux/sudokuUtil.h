#ifndef SUDOKU_UTIL_H
#define SUDOKU_UTIL_H


#include <stdlib.h>


typedef struct SudokuGrid
{
    int** grid;
    int gS;
} SudokuGrid;

int charToInt(char str);
int gridSize(const char* filename);
char intToChar(int nb);
void copyGrid(SudokuGrid sG1, SudokuGrid sG2);
void printGrid(SudokuGrid sG);
SudokuGrid loadGrid(const char *filename);
int saveGrid(const char *filename, SudokuGrid sG);
void freeGrid(SudokuGrid sG);
int** allocGrid(int gS);
char* concateStr(char* a1, char* a2);
void printSection(char* sectionName);

#endif