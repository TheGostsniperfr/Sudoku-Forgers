#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H  

#include <err.h>
#include <stdio.h>


int loadGrid(const char *filename, int** grid, int gS);
void saveMatrix(const char *filename, int** grid, int gS);
int sudokuSolver(int** grid, int gS);
void printGrid(int** grid, int gS);

#endif