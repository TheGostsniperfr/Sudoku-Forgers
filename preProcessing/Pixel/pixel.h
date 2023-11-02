#ifndef PIXEL_H
#define PIXEL_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

Uint8 getPixel8(SDL_Surface* surface, int x, int y);
void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
uint32_t getPixel32_t(SDL_Surface *surface, int x, int y);

#endif