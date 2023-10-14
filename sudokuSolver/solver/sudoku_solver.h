#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H  

#include <err.h>
#include <stdio.h>
#include <math.h>
#include "../convertLib/sudokuConvert.h"

int loadGrid(const char *filename, int** grid, int gS);
void saveGrid(const char *filename, int** grid, int gS);
int sudokuSolver(int** grid, int gS);
void printGrid(int** grid, int gS);

void findEmptyCell(int** grid, int gS, int* row, int* col);

#endif