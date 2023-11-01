#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "preProcessing/SDL_Function/sdlFunction.h"

SDL_Surface* applyMedianFilter(SDL_Surface* img);

#endif