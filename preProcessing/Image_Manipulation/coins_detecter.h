#ifndef COINS_DETECTER_H
#define COINS_DETECTER_H   

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include "math.h"

#include "pixel.h"

void detecterCoins(SDL_Surface* surface, int* points);

#endif