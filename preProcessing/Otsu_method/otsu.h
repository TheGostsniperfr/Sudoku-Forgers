#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#ifndef OTSU_H
#define OTSU_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

double findOtsuSigma(double* histogram);
SDL_Surface* imgToBin(SDL_Surface* img, double threshold);


#endif