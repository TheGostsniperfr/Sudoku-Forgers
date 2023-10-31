#ifndef SUDOKU_HANDLE_H
#define SUDOKU_HANDLE_H


#include <string.h>
#include "GUI/handleUtils.h"


int handleSolver(
        int argc,
        char* argv[],
        char* gridPath,
        Flag* flags
    );

int handleConsoleGridPrint(
        int argc,
        char* argv[],
        char* gridPath,
        Flag* flags
    );

int handleConsoleOutputGridPrint(
        int argc,
        char* argv[],
        char* gridPath,
        Flag* flags
    );

int handleGenerateGridImg(
        int argc,
        char* argv[],
        char* gridPath,
        Flag* flags
    );

int handlePrintHelp(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* inputImgPath __attribute__((unused)),
        Flag* flags __attribute__((unused))
    );



#endif