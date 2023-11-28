#ifndef SUDOKU_HANDLE_H
#define SUDOKU_HANDLE_H


#include <string.h>
#include "GUI/handleUtils.h"

typedef struct Option
{
    char* flag;
    int(*action)
        (
            int argc,
            char* argv[],
            char* inputImgPath,
            Flag* flag
        );

} Option;

int handleSolver(
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

int handlePrintSolverHelp(
        int argc __attribute__((unused)),
        char* argv[] __attribute__((unused)),
        char* inputImgPath __attribute__((unused)),
        Flag* flags __attribute__((unused))
    );



#endif