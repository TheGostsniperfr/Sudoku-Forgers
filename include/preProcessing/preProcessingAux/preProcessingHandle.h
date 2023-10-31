#ifndef PRE_PROCESSING_HANDLE_H
#define PRE_PROCESSING_HANDLE_H

#include <string.h>
#include "GUI/handleUtils.h"

typedef struct Flag{
    char* flag;
    int value;
} Flag;



int handleAllSteps(
        int argc,
        char* argv[],
        char* inputImgPath,
        Flag* flags
    );

int handleTurnNDegree(
        int argc,
        char* argv[],
        char* inputImgPath,
        Flag* flags
    );

int handlePrintHelp(
        int argc ,
        char* argv[] ,
        char* inputImgPath ,
        Flag* flags
    );

#endif