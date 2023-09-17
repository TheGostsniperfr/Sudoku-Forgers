#ifndef PRE_PROCESSING_H
#define PRE_PROCESSING_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <err.h>

SDL_Surface* loadImg(char *path);
void saveImg(SDL_Surface* image, char *filename);

#endif