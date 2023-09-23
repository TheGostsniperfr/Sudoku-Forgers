#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H  

#include <err.h>
#include <stdio.h>


int loadGrid(const char *filename, int grid[9][9]);
void saveMatrix(const char *filename, int grid[9][9]);
int loadGrid(const char *filename, int grid[9][9]);
void sudokuSolver(int grid[9][9]);
void printGrid(int grid[9][9]);

#endif