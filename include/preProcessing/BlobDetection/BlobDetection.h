#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include <err.h>

typedef struct Point {
    int x;
    int y;
    int size;
}Point;

int Fill
(
    SDL_Surface *src,
    Uint32* pixels,
    int x,
    int y,
    int* blob,
    char lim
);

SDL_Surface* Blob
(
    SDL_Surface* src, 
    int* size_max
);

#endif