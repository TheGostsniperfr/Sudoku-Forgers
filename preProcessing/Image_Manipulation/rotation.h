#ifndef ROTATION_H
#define ROTATION_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

#include "pixel.h"

SDL_Surface* Rotated_image(SDL_Surface* image, double angle);

#endif