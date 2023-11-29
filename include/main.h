#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>


//PreProcessing
#include "preProcessing/preProcessingAux/preProcessingHandle.h"
#include "preProcessing/Image_Clean/Image_Clean.h"

//network
#include "neuralNetwork/network_Utils/networkHandle.h"
#include "neuralNetwork/network_Utils/networkUtils.h"

//Sudoku solver
#include "sudokuSolver/sudokuAux/sudokuHandle.h"
#include "sudokuSolver/sudokuAux/sudokuUtil.h"
#include "sudokuSolver/sudokuSolver/sudoku_solver.h"

//GUI
#include "GUI/gui.h"



#endif