#ifndef CREATE_OUTPUT_GRID_H
#define CREATE_OUTPUT_GRID_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../preProcessing/SDL_Function/sdlFunction.h"
#include "../sudokuSolver/sudoku_solver.h"
#include <SDL_ttf.h>


SDL_Surface* createOutputGrid(SudokuGrid defaultSG, SudokuGrid solvedSG);

#endif