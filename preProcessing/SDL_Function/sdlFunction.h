#ifndef SDL_FUNCTION_H
#define SDL_FUNCTION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>

SDL_Surface* loadImg(char *path);
void saveImg(SDL_Surface* image, char *filename);
Uint32 getpixel(SDL_Surface *surface, int x, int y);

SDL_Surface* fillRect(SDL_Surface* img, int _x1, int _x2, int _y1, int _y2, Uint32 pixel);
#endif