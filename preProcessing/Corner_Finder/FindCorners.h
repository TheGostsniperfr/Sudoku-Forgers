#ifndef FIND_CORNERS_H
#define FIND_CORNERS_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

#include "../Pixel/pixel.h"

int* FindCoins(SDL_Surface* surface);

#endif