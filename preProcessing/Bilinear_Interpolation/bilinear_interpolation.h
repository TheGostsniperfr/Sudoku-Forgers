#ifndef BILNEAR_INTERPOLATION_H
#define BILNEAR_INTERPOLATION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#include "../Pixel/pixel.h"

SDL_Surface* Bilinear_Interpolation(SDL_Surface* image, int output_width, int output_height);

#endif