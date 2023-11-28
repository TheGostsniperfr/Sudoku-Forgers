#ifndef PRE_PROCESSING_HANDLE_H
#define PRE_PROCESSING_HANDLE_H

#include <string.h>
#include "GUI/handleUtils.h"

typedef struct OptionPrePro
{
    char* flag;
    void* (*action)
        (
            int argc,
            char* argv[],
            char* inputImgPath,
            Flag* flag
        );

} OptionPrePro;

void* handleAllSteps(
        int argc,
        char* argv[],
        char* inputImgPath,
        Flag* flags
    );

void* handleTurnNDegree(
        int argc,
        char* argv[],
        char* inputImgPath,
        Flag* flags
    );

void* handlePrintPreProHelp(
        int argc ,
        char* argv[] ,
        char* inputImgPath ,
        Flag* flags
    );

#endif