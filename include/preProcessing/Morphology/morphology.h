#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "preProcessing/SDL_Function/sdlFunction.h"

#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

SDL_Surface* applyMorphology(SDL_Surface* img, int _mode);


#endif