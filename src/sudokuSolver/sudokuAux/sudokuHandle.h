#ifndef SUDOKU_HANDLE_H
#define SUDOKU_HANDLE_H

#include <string.h>

#include "sudokuSolver/sudokuAux/sudokuUtil.h"
#include "sudokuSolver/sudokuSolver/sudoku_solver.h"


typedef struct Option
{
    char* flag;
    int(*action)(int argc, char* argv[], char* gridPath);
} Option;


int handleSolver(int argc, char* argv[], char* gridPath);
int handleConsoleGridPrint(int argc, char* argv[], char* gridPath);
int handleConsoleOutputGridPrint(int argc, char* argv[], char* gridPath);
int handleGenerateGridImg(int argc, char* argv[], char* gridPath);


#endif