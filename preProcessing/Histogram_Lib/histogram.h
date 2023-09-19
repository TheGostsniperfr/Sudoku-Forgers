#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#ifndef HISTOGRAM_H
#define HISTOGRAM_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

int* findHistogram(SDL_Surface* img);
SDL_Surface* createHistogramImg(int* histogram);

#endif