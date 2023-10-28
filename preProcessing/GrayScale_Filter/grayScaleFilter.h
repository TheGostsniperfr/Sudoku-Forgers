#ifndef GRAY_SCALE_FILTER_H
#define GRAY_SCALE_FILTER_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

SDL_Surface* applyGrayScaleFilter(SDL_Surface* inputSurface);

#endif