#ifndef SDL_FUNCTION_H
#define SDL_FUNCTION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>

SDL_Surface* loadImg(char *path);
void saveImg(SDL_Surface* image, char *filename);

#endif