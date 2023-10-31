#ifndef ADAPTIVE_THRESHOLD_H
#define ADAPTIVE_THRESHOLD_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

typedef struct VarMap{
    int width;
    int height;
    double** data;
} VarMap;


SDL_Surface* applyAdaptiveThreshold(SDL_Surface* inputImg);
VarMap* createVarMap(SDL_Surface* img);
#endif