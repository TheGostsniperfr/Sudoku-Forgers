#ifndef ROTATION_H
#define ROTATION_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

#include "../Pixel/pixel.h"
#include "../Corner_Finder/FindCorners.h"

SDL_Surface* Rotated_image(SDL_Surface* image, double angle);
void Save_RotatedImg(SDL_Surface* image, double angle);

#endif