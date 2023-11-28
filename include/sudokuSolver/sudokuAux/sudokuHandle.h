#ifndef SUDOKU_HANDLE_H
#define SUDOKU_HANDLE_H


#include <string.h>
#include "GUI/handleUtils.h"

typedef struct OptionSolver
{
    char* flag;
    void* (*action)
        (
            int argc,
            char* argv[],
            char* inputImgPath,
            Flag* flag
        );

} OptionSolver;

void* handleSolver(
        int argc,
        char* argv[],
        char* gridPath,
        Flag* flags
    );

void* handleGenerateGridImg(
        int argc,
        char* argv[],
        char* gridPath,
        Flag* flags
    );

void* handlePrintSolverHelp(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* inputImgPath __attribute__((unused)),
        Flag* flags __attribute__((unused))
    );



#endif