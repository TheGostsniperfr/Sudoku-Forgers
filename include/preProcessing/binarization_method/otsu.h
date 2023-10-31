#ifndef OTSU_H
#define OTSU_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

double findOtsuThreshold(double* histogram, int numPixels);
SDL_Surface* otsuBinarization(SDL_Surface* img, double threshold);


#endif