#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <err.h>
#include <stdio.h>
#include <math.h>
#include "../sudokuAux/sudokuUtil.h"

int sudokuSolver(SudokuGrid sG);

void findEmptyCell(SudokuGrid sG, int* row, int* col);

#endif