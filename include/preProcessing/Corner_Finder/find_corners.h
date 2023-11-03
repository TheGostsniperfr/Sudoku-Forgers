#ifndef FIND_CORNERS_H
#define FIND_CORNERS_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include "preProcessing/Matrix/mat.h"

int* FindCoins(SDL_Surface* surface);

#endif