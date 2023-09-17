#ifndef GAUSSIAN_FILTER_H
#define GAUSSIAN_FILTER_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../SDL_Function/sdlFunction.h"

SDL_Surface* applyGaussianFilter(SDL_Surface* img);


#endif