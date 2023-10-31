#ifndef HANDLE_UTILS_H
#define HANDLE_UTILS_H


#define ERROR_NB_ARG "⚠️ Number of arguments does not match. ⚠️"

typedef struct Flag{
    char* flag;
    int value;
} Flag;

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


#endif