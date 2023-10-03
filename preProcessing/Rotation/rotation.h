#ifndef ROTATION_H
#define ROTATION_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

#include "../Pixel/pixel.h"
#include "../Corner_Finder/coins_detecter.h"

SDL_Surface* Rotated_image(SDL_Surface* image, double angle);
double Find_angle_to_Rotate(SDL_Surface* image);

#endif