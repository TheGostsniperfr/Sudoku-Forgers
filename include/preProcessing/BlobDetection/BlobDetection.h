#ifndef BLOBDETECTION_H
#define BLOBDETECTION_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"
#include <err.h>

void Blob(SDL_Surface* src);

int Fill
(
    SDL_Surface *src,
    Uint32* pixels,
    int x,
    int y,
    Uint8 R,
    Uint8 G,
    Uint8 B,
    char Func
);

#endif